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
#include "cache_container.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class element
{
public:
    template <typename T> constexpr inline
    auto cast(T&& v) { return utility::cast(std::forward<T>(v)); } // aliasing utility::cast function
    std::string name_;
protected:
    cache_container<std::string, element> &all_elements_;
    camera& cam_;
    SDL_Renderer *renderer_ {nullptr};
    SDL_Texture_ptr texture_ {nullptr, &SDL_DestroyTexture};
    std::unique_ptr<animation> anime_info_;
    std::unique_ptr<fps>* game_fps_ {nullptr};

    int current_frame_col_ = 0;
    int current_frame_row_ = 0;

public:
    SDL_Rect src_ {};
    bool   valid_ {true};

    pixel max_speed_x_ = 20;
    pixel max_speed_y_ = 80;
    pixel col_offset_ = 5;
    unsigned int z_index_ = 0; // higher z_index_ become, element are more to front when rendering

public:
    enum class type
    {
        none    = 0,
        generic = 1 << 1,
        text    = 1 << 2,
        ball    = 1 << 3,
        player  = 1 << 4,
        counter = 1 << 5,
        fence   = 1 << 6
    };
    type type_ = type::generic;

public:
    /* move_calculation collide system:
       1. => is in ingore list
       2. => is in collides list
       3. check hardness
       hardness
       => air(1) cannot pass player(50)
       => player are not effected by air
       => if same, effect to both side
     */
    enum class hardness
    {
        air     = 1,
        stone   = 50,
        diamond = 100
    };
    hardness hardness_   = hardness::diamond;
    type collides_types_ = type::none;
    type ignore_types_   = type::none;

    enum class flag
    {
        none     = utility::compile_time::pow<0>,
        gravity  = utility::compile_time::pow<1>,
        ghost    = utility::compile_time::pow<2>,
        map_only = utility::compile_time::pow<3>
    };
    flag flag_ = flag::none;

    enum class bounce_direction {stop    = 0,
                                 reverse = 1,
                                 same    = 2};
    bounce_direction bounce_x_ = bounce_direction::stop,
                     bounce_y_ = bounce_direction::stop;

    struct state
    {
        rect<> dest_ {};
        pixel speed_x_ = 0;     // pixel per second
        pixel speed_y_ = 0;     // pixel per second
        pixel old_speed_x_ = 0; // speed of previous frame that can used in on_collision
        pixel old_speed_y_ = 0; // speed of previous frame that can used in on_collision
        pixel accel_x_ = 0;     // pixel^2 per second
        pixel accel_y_ = 10;    // pixel^2 per second
    };
    state state_;

    struct collision
    {
        static
        std::deque<collision>& queue() { static std::deque<collision> q; return q; }

        element & A;
        element & B;
    };

public:
    element(std::string_view element_name,
            cache_container<std::string, element> &all,
            SDL_Renderer *r,
            camera &c,
            std::unique_ptr<fps>* game_fps):
        name_         {element_name},
        all_elements_ {all},
        cam_          {c},
        renderer_     {r},
        game_fps_     {game_fps} {}

    virtual ~element() = default;

    virtual
    void calculate()
    {
        state_.speed_x_ += state_.accel_x_ * (*game_fps_)->speed_factor();
        state_.speed_y_ += state_.accel_y_ * (*game_fps_)->speed_factor();

        move_calculate();

        animate();
    }

    virtual
    void render()
    {
        auto [camera_x, camera_y] = cam_.get_pos();

        SDL_Rect pos = state_.dest_;
        pos.x -= camera_x;
        pos.y -= camera_y;

        SDL_RenderCopy (renderer_, texture_.get(), &src_, &pos);
    }

    virtual
    void animate()
    {
        anime_info_->animate();
        src_.x = src_.w *  current_frame_col_;
        src_.y = src_.h * (current_frame_row_ + anime_info_->get_current_frame());
    }

    virtual void on_collision (element &) {}

    virtual
    void build_from_toml(std::shared_ptr<cpptoml::table> table)
    {
        build_from_toml_basic(table);

        if (error_code ec = set_texture_from_picture (
                *(table->get_as<std::string>("pic")),
                table->get_as<int>("picwidth").value_or(src_.w),
                table->get_as<int>("picheight").value_or(src_.h),
                static_cast<animation::rotate_type>(table->get_as<int>("rotate_t")
                                                    .value_or(cast(animation::rotate_type::none)))); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (auto rate = table->get_as<double>("amplify"); rate)
            amplify(*rate);
    }

    virtual void on_key_down(SDL_Keycode const &, Uint16 const &) {}

    virtual void on_key_up(SDL_Keycode const &, Uint16 const &) {}


protected:
    void move_calculate()
    {
        if (state_.speed_x_ == 0 and state_.speed_y_ == 0)
            return;

        utility::trim    (state_.speed_x_,  max_speed_x_);
        utility::trim_inv(state_.speed_x_, -max_speed_x_);

        utility::trim    (state_.speed_y_,  max_speed_y_);
        utility::trim_inv(state_.speed_y_, -max_speed_y_);

        state_.old_speed_x_ = state_.speed_x_;
        state_.old_speed_y_ = state_.speed_y_;

        double speed_factor = (*game_fps_)->speed_factor();
        double vx = state_.speed_x_ * speed_factor;
        double vy = state_.speed_y_ * speed_factor;

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
            if (bool_of(flag_ & flag::ghost))
            {
                state_.dest_.x += x_shift_step;
                state_.dest_.y += y_shift_step;

                // send collisions
                is_pos_valid(state_.dest_.x, state_.dest_.y);
            }
            else
            {
                if (x_shift_step != 0)
                {
                    if (is_pos_valid(state_.dest_.x + x_shift_step, state_.dest_.y))
                        state_.dest_.x += x_shift_step;
                    else
                        state_.speed_x_ = 0;
                }

                if (y_shift_step != 0)
                {
                    if (is_pos_valid(state_.dest_.x, state_.dest_.y + y_shift_step))
                        state_.dest_.y += y_shift_step;
                    else
                        state_.speed_y_ = 0;
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

    // setup: x, y, flag, offset, bounce_x, bounce_y, accel_y, z_index, hardness
    void build_from_toml_basic(std::shared_ptr<cpptoml::table> table)
    {
        state_.dest_.x  = table->get_as<pixel>("x").value_or(state_.dest_.x);
        state_.dest_.y  = table->get_as<pixel>("y").value_or(state_.dest_.y);
        flag_ = static_cast<element::flag>(table->get_as<int>("flag")
                                           .value_or(cast(flag_)));
        col_offset_ = table->get_as<pixel>("offset").value_or(col_offset_);

        bounce_x_ = static_cast<bounce_direction>(table->get_as<int>("bounce_x")
                                                  .value_or(cast(bounce_x_)));
        bounce_y_ = static_cast<bounce_direction>(table->get_as<int>("bounce_y")
                                                  .value_or(cast(bounce_y_)));

        hardness_ = static_cast<element::hardness>(table->get_as<int>("hardness")
                                                   .value_or(cast(hardness_)));

        z_index_ = table->get_as<unsigned int>("z_index").value_or(z_index_);

        if (not bool_of(flag_ & flag::gravity))
            state_.accel_y_ = 0;
    }

    error_code set_texture(SDL_Surface_ptr surface,
                           pixel w, pixel h,
                           animation::rotate_type t)
    {
        texture_.reset (SDL_CreateTextureFromSurface (renderer_, surface.get()));

        src_.w = state_.dest_.w = w;
        src_.h = state_.dest_.h = h;
        int pic_frame = surface->h / h;
        anime_info_ = std::make_unique<animation>(pic_frame, t);
        return 0;
    }

public:
    void stop_move()
    {
        if (state_.speed_x_ > 0)
            state_.accel_x_ = -8;

        if (state_.speed_x_ < 0)
            state_.accel_x_ = 8;

        if (-16 < state_.speed_x_ && state_.speed_x_ < 16)
            state_.accel_x_ = state_.speed_x_ = 0;
    }

    // this function amplify this element, and then adjust dest.x value
    // to make middle point of the element won't change

    enum class amplify_mode {top_left, center};
    void amplify (double multiple,
                  std::optional<double> h = std::nullopt,
                  std::optional<amplify_mode> mode = std::nullopt)
    {
        pixel old_w = state_.dest_.w;
        pixel old_h = state_.dest_.h;
        state_.dest_.w *= multiple;
        state_.dest_.h = (h)? (state_.dest_.h * (*h)) : (src_.h * state_.dest_.w / src_.w);
        if (mode.value_or(amplify_mode::center) == amplify_mode::center)
        {
            state_.dest_.x -= (state_.dest_.w - old_w) / 2;
            state_.dest_.y -= (state_.dest_.h - old_h) / 2;
        }
    }

    void amplify_to (pixel w,
                     std::optional<pixel> h = std::nullopt,
                     std::optional<amplify_mode> mode = std::nullopt)
    {
        if (h)
            amplify(w/state_.dest_.w, *h/state_.dest_.h, mode);
        else
            amplify(w/state_.dest_.w, std::nullopt, mode);
    }

    error_code set_alpha(std::uint8_t alpha)
    {
        return SDL_SetTextureAlphaMod(texture_.get(), alpha);
    }

    void bind_fps(std::unique_ptr<fps> * new_heart) { game_fps_ = new_heart; }

    template <typename T>
    bool collides_with (rect<T> const & r)
    {
        SDL_Rect col {
            .x = static_cast<int>(state_.dest_.x + col_offset_),
            .y = static_cast<int>(state_.dest_.y + col_offset_),
            .w = static_cast<int>(col_w()),
            .h = static_cast<int>(col_h())
        };
        SDL_Rect rhs {r.to_sdl()};
        return SDL_HasIntersection(&rhs, &col) == SDL_TRUE;
    }

    point<> mid_point()
    {
        return {
            .x = state_.dest_.x + state_.dest_.w/2,
            .y = state_.dest_.y + state_.dest_.h/2
        };
    }

    inline pixel col_w() { return state_.dest_.w - col_offset_ * 2; }
    inline pixel col_h() { return state_.dest_.h - col_offset_ * 2; }

private:
    error_code set_texture_from_picture(std::string_view path,
                                        pixel pic_w, pixel pic_h,
                                        animation::rotate_type t = animation::rotate_type::none)
    {
        SDL_Surface_ptr image {IMG_Load(path.data()), &SDL_FreeSurface};
        if (image == nullptr)
        {
            std::cout << IMG_GetError() << std::endl;
            return -1;
        }
        return set_texture(std::move(image), pic_w, pic_h, t);
    }


    bool is_pos_valid (pixel new_x, pixel new_y)
    {
        SDL_Point top_left_map  = { .x = static_cast<int>(new_x + col_offset_          ) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset_          ) / TILE_SIZE_PIXEL_INT};
        SDL_Point top_right_map = { .x = static_cast<int>(new_x + col_offset_ + col_w()) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset_          ) / TILE_SIZE_PIXEL_INT};
        SDL_Point down_left_map = { .x = static_cast<int>(new_x + col_offset_          ) / TILE_SIZE_PIXEL_INT,
                                    .y = static_cast<int>(new_y + col_offset_ + col_h()) / TILE_SIZE_PIXEL_INT};

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

        if (not bool_of(flag_ & flag::map_only))
            for (auto &e : all_elements_)
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
            e->valid_ and
            bool_of(e->flag_ ^ flag::map_only) /* doesn't have the ENTITY_FLAG_MAPONLY flag turned on */ and
            e->collides_with(rect<pixel>{
                .x = x + col_offset_,
                .y = y + col_offset_,
                .w = col_w(),
                .h = col_h()
           }))
        {
            collision::queue().push_back(collision{ .A = *this, .B = *e });

            // detail description are at enum class hardness
            if (bool_of(ignore_types_   & e->type_))
                return true;

            if (bool_of(collides_types_ & e->type_))
                return false;

            return hardness_ > e->hardness_;
        }
        return true;
    }
};

}// namespace game
#endif // ELEMENT_HPP_
