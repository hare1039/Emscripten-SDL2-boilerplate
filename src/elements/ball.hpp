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
        type_ = type::ball;
    }

    next_operation on_collision(element & e) override
    {
        if (e.type_ != type::player)
        {
            if (e.bounce_x_ == bounce_direction::reverse)
                state_.speed_x_ = -state_.old_speed_x_;

            if (e.bounce_y_ == bounce_direction::reverse)
                state_.speed_y_ = -state_.old_speed_y_;
        }
        return next_operation::cont;
    }
};

} // namespace game::element_types

#endif // ELEMENTS_BALL_HPP_
