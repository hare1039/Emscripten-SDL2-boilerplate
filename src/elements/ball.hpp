#ifndef ELEMENTS_BALL_HPP_
#define ELEMENTS_BALL_HPP_

#include "../element.hpp"
#include "player.hpp"
#include "score_counter.hpp"

namespace game::elements::types
{

class ball : public element
{
public:
    ball(SDL_Renderer *r,
         std::string_view name,
         cache_container<std::string, element> &a,
         camera &c,
         std::unique_ptr<fps>* game_fps): element {r, name, a, c, game_fps}
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
                dynamic_cast<elements::types::score_counter *>(&e)->increase_once();
        }
    }
};

} // namespace game::elements::types

#endif // ELEMENTS_BALL_HPP_
