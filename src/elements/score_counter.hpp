#ifndef ELEMENTS_SCORE_COUNTER_HPP_
#define ELEMENTS_SCORE_COUNTER_HPP_
#pragma once

#include "../basic_headers.hpp"
#include "obstacle.hpp"

namespace game::element_types
{

class score_counter : public obstacle
{
    unsigned int collision_count_ = 0;
    bool         ball_in_ = false;
public:
    score_counter(SDL_Renderer *r,
                  std::string_view name,
                  std::unordered_map<std::string, std::unique_ptr<element>> &a,
                  camera &c): obstacle {r, name, a, c}
    {
        type_ = type::counter;
    }

    void increase() { collision_count_++; }
    void increase_once()
    {
        if (not ball_in_)
        {
            ball_in_ = true;
            collision_count_++;
        }
    }

    unsigned int count() { return collision_count_; }

    void on_collision(element &e) override
    {
        ball_in_ = (e.type_ == type::ball);
    }
};

} // namespace game::element_types

#endif // ELEMENTS_SCORE_COUNTER_HPP_
