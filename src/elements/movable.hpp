#ifndef ELEMENTS_MOVABLE_HPP_
#define ELEMENTS_MOVABLE_HPP_
#pragma once

#include <string_view>
#include "../basic_headers.hpp"
#include "../element.hpp"

namespace game::element_types
{

class movable : public element
{
public:
    bool jumpable_   = false;
    bool move_right_ = false;
    bool move_left_  = false;
    bool dead_       = false;

public:
    movable(SDL_Renderer *r,
            std::string_view name,
            cache_container<std::string, element> &a,
            camera &c): element {r, name, a, c} {}

    void animate() override
    {
        if (move_left_)
            current_frame_col_ = 0;
        if (move_right_)
            current_frame_col_ = 1;

        element::animate();

        if (state_.speed_x_ == 0)
            anime_info_->set_current_frame(0).lock();
        else
            anime_info_->unlock();
    }

    void calculate() override
    {
        if (not move_left_ and not move_right_)
            stop_move();

        if (move_left_)
            state_.speed_x_ = -max_speed_x_;

        if (move_right_)
            state_.speed_x_ =  max_speed_x_;

        pixel old_y = state_.dest_.y;

        element::calculate();
        if (not jumpable_ and state_.dest_.y == old_y)
            try
            {
                pixel left_x   = (state_.dest_.x + col_offset_)                  / TILE_SIZE_PIXEL;
                pixel right_x  = (state_.dest_.x + col_offset_ + col_w())        / TILE_SIZE_PIXEL;
                pixel target_y = (state_.dest_.y + col_offset_ + state_.dest_.h) / TILE_SIZE_PIXEL;
                if (area::instance()->at_map(left_x,  target_y).is_solid() or
                    area::instance()->at_map(right_x, target_y).is_solid())
                    jumpable_ = true;
            } catch (std::out_of_range const &e) { /* expected exception */ }
    }

    bool jump()
    {
        if (not jumpable_)
            return false;

        state_.speed_y_ = -max_speed_y_;
        jumpable_ = false;
        return true;
    }

    bool& alive() {return valid_;}
};

} // namespace game::element_types

#endif // ELEMENTS_MOVABLE_HPP_
