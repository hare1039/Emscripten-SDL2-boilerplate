#ifndef ELEMENTS_BALL_HPP_
#define ELEMENTS_BALL_HPP_

#include "../element.hpp"
#include "player.hpp"
#include "score_counter.hpp"

namespace game::element_types
{

class ball : public element
{
public:
    ball(SDL_Renderer *r,
         std::string_view name,
         std::unordered_map<std::string, std::unique_ptr<element>> &a,
         camera &c): element {r, name, a, c}
    {
        max_speed_x_ = 60;
        max_speed_y_ = 60;
        type_     = type::ball;
        hardness_ = hardness::stone;
    }

    void on_collision(element & e) override
    {
        if (e.type_ != type::player)
        {
            if (e.bounce_x_ == bounce_direction::reverse)
                state_.speed_x_ = -state_.old_speed_x_;

            if (e.bounce_y_ == bounce_direction::reverse)
                state_.speed_y_ = -state_.old_speed_y_;

            if (e.type_ == type::counter)
                dynamic_cast<element_types::score_counter *>(&e)->increase_once();
        }
    }
};

} // namespace game::element_types

#endif // ELEMENTS_BALL_HPP_
