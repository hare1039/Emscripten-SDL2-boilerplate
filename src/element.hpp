#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <limits>
#include <cassert>
#include "animation.hpp"
#include "area.hpp"
#include "basic_headers.hpp"
#include "camera.hpp"
#include "fps.hpp"
#include "tile.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class element
{
    struct point { int x = 0, y = 0; };
    template <typename T> constexpr inline
    auto cast(T&& v) { return utility::cast(std::forward<T>(v)); } // aliasing utility::cast function
protected:
    std::string name;
    std::unordered_map<std::string, std::unique_ptr<element>> &all_elements;
    camera& cam;

    SDL_Renderer   *renderer = nullptr;
    SDL_Texture_ptr texture {nullptr, &SDL_DestroyTexture};
    std::unique_ptr<animation> anime_info;

    double speed_x = 0; // pixel per second
    double speed_y = 0; // pixel per second

    double accel_x = 0; // pixel^2 per second
    double accel_y = 0; // pixel^2 per second

    pixel col_offset = 10;

    int current_frame_col = 0;
    int current_frame_row = 0;
    bool jumpable = false;

public:
    SDL_Rect dest = {}, src = {};
    bool move_right = false;
    bool move_left  = false;
    bool dead = false;

    double max_speed_x = 10;
    double max_speed_y = 15;

public:
    enum class type
    {
        generic = 0,
        player
    } type_id = type::generic;

    enum class flag
    {
        none     = 0,
        gravity  = 1,
        ghost    = 1 << 1,
        map_only = 1 << 2
    } flag_id = flag::none;

    struct collision
    {
        static
        std::deque<collision>& queue() { static std::deque<collision> q; return q; }

        element & A;
        element & B;
    };

public:
    element(SDL_Renderer *r,
            std::string_view element_name,
            std::unordered_map<std::string, std::unique_ptr<element>> &all,
            camera &c):
        name {element_name},
        all_elements{all},
        cam{c},
        renderer{r} {}

    virtual ~element() = default;

    virtual
    void calculate()
    {
        if (not move_left and not move_right)
            stop_move();

        if (move_left)
            accel_x = -0.5;

        if (move_right)
            accel_x = 0.5;

        if (cast(flag_id) & cast(flag::gravity))
            accel_y = 0.75;

        speed_x += accel_x * fps::instance()->speed_factor();
        speed_y += accel_y * fps::instance()->speed_factor();

        if (speed_x >  max_speed_x) speed_x =  max_speed_x;
        if (speed_x < -max_speed_x) speed_x = -max_speed_x;
        if (speed_y >  max_speed_y) speed_y =  max_speed_y;
        if (speed_y < -max_speed_y) speed_y = -max_speed_y;

        animate();

        jumpable = false;
        int old_y = dest.y;
        move_calculate (speed_x, speed_y);
        if (dest.y == old_y)
            try
            {
                int left_x   = (dest.x + col_offset) / TILE_SIZE_PIXEL;
                int right_x  = (dest.x + col_offset + col_w()) / TILE_SIZE_PIXEL;
                int target_y = (dest.y + col_offset + dest.h) / TILE_SIZE_PIXEL;
                if (area::instance()->at_map(left_x, target_y).is_solid() or
                    area::instance()->at_map(right_x, target_y).is_solid())
                    jumpable = true;
            } catch (std::out_of_range const &e) { /* expected exception */ }
    }

    virtual
    void render()
    {
        auto [camera_x, camera_y] = cam.get_pos();
        SDL_Rect pos = {
            .x = dest.x - camera_x,
            .y = dest.y - camera_y,
            .w = dest.w,
            .h = dest.h
        };
        SDL_RenderCopy (renderer, texture.get(), &src, &pos);
    }

    virtual
    void animate()
    {
        anime_info->animate();
        if (move_left)
            current_frame_col = 0;
        if (move_right)
            current_frame_col = 1;
        src.x = src.w * current_frame_col;
        src.y = src.h * (current_frame_row + anime_info->get_current_frame());
    }

    virtual
    next_operation on_collision (element &) { return next_operation::cont; }

    virtual
    void build_from_toml(std::shared_ptr<cpptoml::table> table)
    {
        dest.x  = table->get_as<pixel>("x").value_or(dest.x);
        dest.y  = table->get_as<pixel>("y").value_or(dest.y);
        flag_id = static_cast<element::flag>(table->get_as<int>("flag_id")
                                             .value_or(cast(flag_id)));
        col_offset = table->get_as<pixel>("offset").value_or(col_offset);

        if (error_code ec = set_texture(
                *(table->get_as<std::string>("pic")),
                table->get_as<int>("width").value_or(src.w),
                table->get_as<int>("height").value_or(src.h),
                static_cast<animation::rotate_type>(table->get_as<int>("rotate_t")
                                                    .value_or(cast(animation::rotate_type::none)))); ec < 0)
            std::cout << SDL_GetError() << std::endl;
    }

    virtual
    error_code set_texture(std::string_view path,
                           pixel pic_w, pixel pic_h,
                           animation::rotate_type t = animation::rotate_type::none)
    {
        SDL_Surface_ptr image {IMG_Load(path.data()), &SDL_FreeSurface};
        if (not image)
        {
            std::cout << IMG_GetError() << std::endl;
            return -1;
        }

        texture.reset (SDL_CreateTextureFromSurface (renderer, image.get()));

        src.w = dest.w = pic_w;
        src.h = dest.h = pic_h;
        int pic_frame = image->h / pic_h;
        anime_info = std::make_unique<animation>(pic_frame, t);
        return 0;
    }

public:
    void move_calculate(double vx, double vy)
    {
        if (vx == 0 && vy == 0)
            return;

        double speed_factor = fps::instance()->speed_factor();
        vx *= speed_factor;
        vy *= speed_factor;

        if (vy > max_speed_y * max_speed_y)
            vy = 0;

        double x_shift = vx;
        double y_shift = vy;

        do
        {
            if (cast(flag_id) & cast(flag::ghost))
            {
                // send collisions
                is_pos_valid(dest.x + x_shift, dest.y + y_shift);
                dest.x += x_shift;
                dest.y += y_shift;
            }
            else
            {
                if (is_pos_valid(dest.x + x_shift, dest.y))
                    dest.x += x_shift;
                else
                    speed_x = 0;

                if (is_pos_valid(dest.x, dest.y + y_shift))
                    dest.y += y_shift;
                else
                    speed_y = 0;
            }

            vx -= x_shift;
            vy -= y_shift;

            if (x_shift > 0 && vx <= 0)
                x_shift = 0;
            if (x_shift < 0 && vx >= 0)
                x_shift = 0;
            if (y_shift > 0 && vy <= 0)
                y_shift = 0;
            if (y_shift > 0 && vy <= 0)
                y_shift = 0;

            if (vx == 0)
                x_shift = 0;
            if (vy == 0)
                y_shift = 0;
        } while (not ((vx == 0 and vy == 0) or
                      (x_shift == 0 and y_shift == 0)));
    }
    void stop_move()
    {
        if (speed_x > 0)
            accel_x = -1;

        if (speed_x < 0)
            accel_x = 1;

        if (-2 < speed_x && speed_x < 2)
            accel_x = speed_x = 0;
    }

    bool jump()
    {
        if (not jumpable)
            return false;

        speed_y = -max_speed_y;
        return true;
    }

    // this function amplify this element, and then adjust dest.x value
    // to make middle point of the element won't change
    void amplify(double w, std::optional<double> h = std::nullopt)
    {
        pixel old_w = dest.w;
        pixel old_h = dest.h;
        dest.w *= w;
        dest.h = (h)? (dest.h * (*h)) : (static_cast<double>(src.h) * dest.w / src.w);
        dest.x -= (dest.w - old_w) / 2;
        dest.y -= (dest.h - old_h) / 2;
    }

    void amplify(pixel w, std::optional<pixel> h = std::nullopt)
    {
        if (h)
            amplify(static_cast<double>(w)/dest.w, static_cast<double>(*h)/dest.h);
        else
            amplify(static_cast<double>(w)/dest.w);
    }

    bool collides_with (SDL_Rect const & r)
    {
        point top_left   = { .x = r.x,       .y = r.y };
        point top_right  = { .x = r.x + r.w, .y = r.y };
        point down_left  = { .x = r.x,       .y = r.y + r.h };
        point down_right = { .x = r.x + r.w, .y = r.y + r.h };

        SDL_Rect col = { .x = dest.x + col_offset, .y = dest.y + col_offset, .w = col_w(), .h = col_h() };

        return within_range(col, top_left)
            || within_range(col, top_right)
            || within_range(col, down_left)
            || within_range(col, down_right);
    }
private:
    static inline
    bool within_range (SDL_Rect const & s, point const & p)
    {
        return ((s.x < p.x && p.x < s.x + s.w) &&
                (s.y < p.y && p.y < s.y + s.h));
    }
    bool is_pos_valid (pixel new_x, pixel new_y)
    {
        point top_left_map   = { .x = (new_x + col_offset          ) / TILE_SIZE_PIXEL,
                                 .y = (new_y + col_offset          ) / TILE_SIZE_PIXEL};
        point top_right_map  = { .x = (new_x + col_offset + col_w()) / TILE_SIZE_PIXEL,
                                 .y = (new_y + col_offset          ) / TILE_SIZE_PIXEL };
        point down_left_map  = { .x = (new_x + col_offset          ) / TILE_SIZE_PIXEL,
                                 .y = (new_y + col_offset + col_h()) / TILE_SIZE_PIXEL };

        bool ok = true; // we can't early return here
        for (int x_i = top_left_map.x; x_i <= top_right_map.x; x_i++)
        {
            for (int y_i = top_left_map.y; y_i <= down_left_map.y; y_i++)
            {
                try
                {
                    tile& t = area::instance()->at_map(x_i, y_i);
                    if (not is_tile_pos_valid(t))
                    {
                        ok = false;
                        break;
                    }
                } catch(std::out_of_range const &e) { /* expected exception */ }
            }
        }

        if (not (cast(flag_id) & cast(flag::map_only)))
            for (auto &e : all_elements)
                if (not is_element_pos_valid(e.second.get(), new_x, new_y))
                    ok = false;

        return ok;
    }
    bool is_tile_pos_valid (tile &t)
    {
        return not t.is_solid();
    }
    bool is_element_pos_valid (element *e, pixel x, pixel y)
    {
        if (this != e and
            not e->dead and
            cast(e->flag_id) ^ cast(flag::map_only) and
            e->collides_with(SDL_Rect{
                .x = x + col_offset,
                .y = y + col_offset,
                .w = col_w(),
                .h = col_h()
           }))
        {
            collision::queue().push_back(collision{ .A = *this, .B = *e });
            return false;
        }

        return true;
    }

    inline pixel col_w() { return dest.w - col_offset * 2; }
    inline pixel col_h() { return dest.h - col_offset * 2; }
};

}// namespace game
#endif // ELEMENT_HPP_
