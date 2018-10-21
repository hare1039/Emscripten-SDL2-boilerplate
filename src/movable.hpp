#ifndef MOVABLE_HPP_
#define MOVABLE_HPP_
#pragma once

#include "element.hpp"

namespace game
{

class movable : public element
{
public:
    movable(SDL_Renderer *r,
            std::string name,
            std::unordered_map<std::string, std::unique_ptr<element>> &a): element{r, name, a} {}

    void animate() override
    {
        element::animate();
        if (speed_x == 0)
            anime_info->set_current_frame(0).lock();
        else
            anime_info->unlock();
    }

    next_operation on_collision (element &e) override { speed_y = -max_speed_y; return element::on_collision(e); }
};

} // namespace game

#endif // MOVABLE_HPP_
