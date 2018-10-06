#ifndef __MOVABLE_HPP__
#define __MOVABLE_HPP__
#pragma once

#include "element.hpp"


namespace game
{

using velocity = int;
enum input_state
{
    NOTHING_PRESSED = 0,
    UP_PRESSED      = 1,
    DOWN_PRESSED    = 1 << 1,
    LEFT_PRESSED    = 1 << 2,
    RIGHT_PRESSED   = 1 << 3
};

constexpr double SPEED_DOWN_RATE = 1.3; //1.4142135624;
constexpr double INV = 1.0 / SPEED_DOWN_RATE;


class movable : public element
{
    unsigned int    active_state = NOTHING_PRESSED;
    velocity        vx = 0;
    velocity        vy = 0;
public:
    movable(SDL_Renderer *r): element{r} {}

    void handle_event(SDL_Event& event) override
    {
        switch (event.type)
        {
        case SDL_QUIT:
//            should_continue = false;
            break;
        case SDL_MOUSEMOTION:
//            std::cout << "mouse moved" << std::endl;
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= UP_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= UP_PRESSED;
                break;

            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= DOWN_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= DOWN_PRESSED;
                break;

            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= LEFT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= LEFT_PRESSED;
                break;

            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    active_state |= RIGHT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    active_state ^= RIGHT_PRESSED;
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

    void calculate() override
    {
        element::calculate();
        if (active_state & UP_PRESSED)
            vy = -50;
        if (active_state & DOWN_PRESSED)
            vy =  50;
        if (active_state & LEFT_PRESSED)
            vx = -50;
        if (active_state & RIGHT_PRESSED)
            vx =  50;

        vx *= INV;
        vy *= INV;

        dest.x += vx;
        dest.y += vy;
    }
};

} // namespace game

#endif // __MOVABLE_HPP__
