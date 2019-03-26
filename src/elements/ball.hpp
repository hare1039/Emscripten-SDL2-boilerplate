#ifndef ELEMENTS_BALL_HPP_
#define ELEMENTS_BALL_HPP_

#include "../element.hpp"
#include "player.hpp"
#include "obstacle.hpp"

namespace game::element_types
{

class ball : public element
{
    bounce_direction next_bounce_x = bounce_direction::stop,
                     next_bounce_y = bounce_direction::stop;
public:
    ball(SDL_Renderer *r,
         std::string_view name,
         std::unordered_map<std::string, std::unique_ptr<element>> &a,
         camera &c): element {r, name, a, c}
    {
        max_speed_x = 60;
        max_speed_y = 90;
    }

    next_operation on_collision(element & e) override
    {
        if (auto * p = dynamic_cast<player*>(&e))
        {
            if (p->dest.x < dest.x)
                speed_x =   p->speed_x + 30;
            else
                speed_x = -(p->speed_x + 30);
            speed_y = -(120 + 5);
        }
        else
        {
            if (next_bounce_x == bounce_direction::stop)
                next_bounce_x = e.bounce_x;

            if (next_bounce_y == bounce_direction::stop)
                next_bounce_y = e.bounce_y;
        }
        return next_operation::cont;
    }

    void calculate() override
    {
        speed_x += accel_x * fps::instance()->speed_factor();
        speed_y += accel_y * fps::instance()->speed_factor();

        std::cout << speed_x << " " << speed_y << static_cast<int>(next_bounce_x) << static_cast<int>(next_bounce_y) << " -> ";
        move_calculate (speed_x, speed_y, next_bounce_x, next_bounce_y);
        std::cout << speed_x << " " << speed_y << "\n";
    }
};

} // namespace game::element_types

#endif // ELEMENTS_BALL_HPP_
