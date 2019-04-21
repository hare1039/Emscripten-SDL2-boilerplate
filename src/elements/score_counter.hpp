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
    bool         ball_in_         = false;
    bool         had_collision_   = false;
public:
    score_counter(SDL_Renderer *r,
                  std::string_view name,
                  cache_container<std::string, element> &a,
                  camera &c,
                  std::unique_ptr<fps>* game_fps): obstacle {r, name, a, c, game_fps}
    {
        type_ = type::counter;
    }

    void increase() { collision_count_++; }
    void decrease() { collision_count_--; }
    void increase_once()
    {
        if (not ball_in_)
        {
            ball_in_ = true;
            collision_count_++;
        }
    }

    unsigned int& count() { return collision_count_; }

    void on_collision(element &e) override
    {
        if (e.type_ == type::ball)
        {
            had_collision_ = true;
            ball_in_ = true;
        }
    }

    void calculate() override
    {
        if (not had_collision_)
            ball_in_ = false;
        had_collision_ = false;
    }
};

} // namespace game::element_types

#endif // ELEMENTS_SCORE_COUNTER_HPP_
