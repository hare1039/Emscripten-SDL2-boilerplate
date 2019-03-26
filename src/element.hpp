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
#include "rect.hpp"
#include "tile.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class element
{
public:
    template <typename T> constexpr inline
    auto cast(T&& v) { return utility::cast(std::forward<T>(v)); } // aliasing utility::cast function
protected:
    std::string name;
    std::unordered_map<std::string, std::unique_ptr<element>> &all_elements;
    camera& cam;

    SDL_Renderer   *renderer = nullptr;
    SDL_Texture_ptr texture {nullptr, &SDL_DestroyTexture};
    std::unique_ptr<animation> anime_info;

    pixel col_offset = 5;

    int current_frame_col = 0;
    int current_frame_row = 0;
    bool jumpable = false;

public:
    rect<> dest {};
    SDL_Rect src {};
    bool move_right = false;
    bool move_left  = false;
    bool dead = false;

    double speed_x = 0; // pixel per second
    double speed_y = 0; // pixel per second

    double accel_x = 0; // pixel^2 per second
    double accel_y = 10; // pixel^2 per second

    double max_speed_x = 20;
    double max_speed_y = 80;

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

    enum class bounce_direction {stop, reverse, same};
    bounce_direction bounce_x = bounce_direction::stop,
                     bounce_y = bounce_direction::stop;

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
            speed_x = -max_speed_x;

        if (move_right)
            speed_x =  max_speed_x;

        pixel old_y = dest.y;
        move_calculate (speed_x, speed_y);

        speed_x += accel_x * fps::instance()->speed_factor();
        speed_y += accel_y * fps::instance()->speed_factor();

        animate();

        if (not jumpable and dest.y == old_y)
            try
            {
                pixel left_x   = (dest.x + col_offset)           / TILE_SIZE_PIXEL;
                pixel right_x  = (dest.x + col_offset + col_w()) / TILE_SIZE_PIXEL;
                pixel target_y = (dest.y + col_offset + dest.h)  / TILE_SIZE_PIXEL;
                if (area::instance()->at_map(left_x,  target_y).is_solid() or
                    area::instance()->at_map(right_x, target_y).is_solid())
                    jumpable = true;
            } catch (std::out_of_range const &e) { /* expected exception */ }
    }

    virtual
    void render()
    {
        auto [camera_x, camera_y] = cam.get_pos();

        SDL_Rect pos = dest;
        pos.x -= camera_x;
        pos.y -= camera_y;

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

        bounce_x = static_cast<bounce_direction>(table->get_as<int>("bounce_x")
                                                 .value_or(cast(bounce_x)));
        bounce_y = static_cast<bounce_direction>(table->get_as<int>("bounce_y")
                                                 .value_or(cast(bounce_y)));

        if (not (cast(flag_id) & cast(flag::gravity)))
            accel_y = 0;

        if (error_code ec = set_texture(
                *(table->get_as<std::string>("pic")),
                table->get_as<int>("picwidth").value_or(src.w),
                table->get_as<int>("picheight").value_or(src.h),
                static_cast<animation::rotate_type>(table->get_as<int>("rotate_t")
                                                    .value_or(cast(animation::rotate_type::none)))); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (auto rate = table->get_as<double>("amplify"); rate)
            amplify(*rate);
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

    virtual
    void on_key_down(SDL_Keycode const &, Uint16 const &) {}

    virtual
    void on_key_up(SDL_Keycode const &, Uint16 const &) {}


public:
    void move_calculate(double vx, double vy,
                        bounce_direction bounce_direction_x = bounce_direction::stop,
                        bounce_direction bounce_direction_y = bounce_direction::stop)
    {
        if (vx == 0 and vy == 0)
            return;

        double speed_factor = fps::instance()->speed_factor();
        vx *= speed_factor;
        vy *= speed_factor;

        double x_shift_step = 0;
        if (vx < 0)
            x_shift_step = -speed_factor;
        else if (vx == 0)
            x_shift_step = 0;
        else
            x_shift_step = speed_factor;

        double y_shift_step = 0;
        if (vy < 0)
            y_shift_step = -speed_factor;
        else if (vy == 0)
            y_shift_step = 0;
        else
            y_shift_step = speed_factor;

        while (not (x_shift_step == 0 and y_shift_step == 0))
        {
            if (cast(flag_id) & cast(flag::ghost))
            {
                dest.x += x_shift_step;
                dest.y += y_shift_step;

                // send collisions
                is_pos_valid(dest.x, dest.y);
            }
            else
            {
                if (x_shift_step != 0)
                {
                    if (is_pos_valid(dest.x + x_shift_step, dest.y))
                        dest.x += x_shift_step;
                    else
                        switch(bounce_direction_x)
                        {
                            case bounce_direction::stop:    speed_x = 0; break;
                            case bounce_direction::reverse: speed_x = -speed_x; break;
                            case bounce_direction::same:    break;
                        }
                }

                if (y_shift_step != 0)
                {
                    if (is_pos_valid(dest.x, dest.y + y_shift_step))
                        dest.y += y_shift_step;
                    else
                        switch(bounce_direction_y)
                        {
                            case bounce_direction::stop:    speed_y = 0; break;
                            case bounce_direction::reverse: speed_y = -speed_y; break;
                            case bounce_direction::same:    break;
                        }
                }
            }

            vx -= x_shift_step;
            vy -= y_shift_step;

            if (x_shift_step * vx <= 0)
                x_shift_step = 0;
            if (y_shift_step * vy <= 0)
                y_shift_step = 0;

            if (vx == 0)
                x_shift_step = 0;
            if (vy == 0)
                y_shift_step = 0;
        }
    }

    void stop_move()
    {
        if (speed_x > 0)
            accel_x = -8;

        if (speed_x < 0)
            accel_x = 8;

        if (-16 < speed_x && speed_x < 16)
            accel_x = speed_x = 0;
    }

    bool jump()
    {
        if (not jumpable)
            return false;

        speed_y = -max_speed_y;
        jumpable = false;
        return true;
    }

    // this function amplify this element, and then adjust dest.x value
    // to make middle point of the element won't change
    void amplify(double multiple, std::optional<double> h = std::nullopt)
    {
        pixel old_w = dest.w;
        pixel old_h = dest.h;
        dest.w *= multiple;
        dest.h = (h)? (dest.h * (*h)) : (src.h * dest.w / src.w);
        dest.x -= (dest.w - old_w) / 2;
        dest.y -= (dest.h - old_h) / 2;
    }

    void amplify_to(pixel w, std::optional<pixel> h = std::nullopt)
    {
        if (h)
            amplify(w/dest.w, *h/dest.h);
        else
            amplify(w/dest.w);
    }

    template <typename T>
    bool collides_with (rect<T> const & r)
    {
        SDL_Rect col {
            .x = static_cast<int>(dest.x + col_offset),
            .y = static_cast<int>(dest.y + col_offset),
            .w = static_cast<int>(col_w()),
            .h = static_cast<int>(col_h())
        };
        SDL_Rect rhs {r.to_sdl()};
        return SDL_HasIntersection(&rhs, &col) == SDL_TRUE;
    }

    point<> mid_point()
    {
        return {
            .x = dest.x + col_offset + col_w()/2,
            .y = dest.y + col_offset + col_h()/2
        };
    }
private:
    bool is_pos_valid (pixel new_x, pixel new_y)
    {
        SDL_Point top_left_map  = { .x = static_cast<int>(new_x + col_offset          ) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset          ) / TILE_SIZE_PIXEL_INT};
        SDL_Point top_right_map = { .x = static_cast<int>(new_x + col_offset + col_w()) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset          ) / TILE_SIZE_PIXEL_INT};
        SDL_Point down_left_map = { .x = static_cast<int>(new_x + col_offset          ) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset + col_h()) / TILE_SIZE_PIXEL_INT};

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
            cast(e->flag_id) ^ cast(flag::map_only) /* doesn't have the ENTITY_FLAG_MAPONLY flag turned on */ and
            e->collides_with(rect<pixel>{
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
