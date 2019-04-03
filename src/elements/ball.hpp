#ifndef ELEMENTS_BALL_HPP_
#define ELEMENTS_BALL_HPP_

#include "../element.hpp"
#include "player.hpp"
#include "obstacle.hpp"

namespace game::element_types
{

class ball : public element
{
    bounce_direction next_bounce_x_ = bounce_direction::stop,
                     next_bounce_y_ = bounce_direction::stop;
    pixel old_speed_x_ = 0,
          old_speed_y_ = 0;
public:
    ball(SDL_Renderer *r,
         std::string_view name,
         std::unordered_map<std::string, std::unique_ptr<element>> &a,
         camera &c): element {r, name, a, c}
    {
        max_speed_x_ = 60;
        max_speed_y_ = 60;
    }

    next_operation on_collision(element & e) override
    {
        if (auto * p = dynamic_cast<player*>(&e))
        {
            if (p->state_.dest_.x + p->state_.dest_.w/2 < state_.dest_.x + state_.dest_.w/2)
                state_.speed_x_ =  p->state_.speed_x_ + 30;
            else
                state_.speed_x_ = -(p->state_.speed_x_ + 30);
            state_.speed_y_ = -(p->state_.speed_y_ + 70);
        }
        else
        {
            if (next_bounce_x_ == bounce_direction::stop)
                next_bounce_x_ = e.bounce_x_;

            if (next_bounce_y_ == bounce_direction::stop)
                next_bounce_y_ = e.bounce_y_;
        }
        return next_operation::cont;
    }

    void calculate() override
    {
        state_.speed_x_ += state_.accel_x_ * fps::instance()->speed_factor();
        state_.speed_y_ += state_.accel_y_ * fps::instance()->speed_factor();

        if (next_bounce_x_ == bounce_direction::reverse)
            state_.speed_x_ = -old_speed_x_;

        if (next_bounce_y_ == bounce_direction::reverse)
            state_.speed_y_ = -old_speed_y_;

        old_speed_x_ = state_.speed_x_;
        old_speed_y_ = state_.speed_y_;
        next_bounce_x_ = next_bounce_y_ = bounce_direction::stop;

        move_calculate (state_.speed_x_, state_.speed_y_);
    }
};

} // namespace game::element_types

#endif // ELEMENTS_BALL_HPP_
