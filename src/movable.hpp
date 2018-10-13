#ifndef MOVABLE_HPP_
#define MOVABLE_HPP_
#pragma once

#include "element.hpp"

namespace game
{

class movable : public element
{
public:
    movable(SDL_Renderer *r): element{r} {}

    void handle_event(SDL_Event& event) override
    {
        switch (event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    jump();
                break;

            case SDLK_DOWN:
                break;

            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    move_left = true;
                else if (event.key.type == SDL_KEYUP)
                    move_left = false;
                break;

            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    move_right = true;
                else if (event.key.type == SDL_KEYUP)
                    move_right = false;
                break;

            case SDLK_SPACE:
                jump();
                break;

            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }

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
