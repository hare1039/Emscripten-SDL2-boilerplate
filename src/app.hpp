#ifndef __APP_HPP__
#define __APP_HPP__
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

#include "basic_headers.hpp"
#include "element.hpp"

namespace game
{

class app
{
    bool is_running = true;
    SDL_Window   * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    
    std::unique_ptr<element> rin;

public:
    app()
    {
        if (error_code ec = SDL_Init(SDL_INIT_VIDEO); ec < 0)
            std::cout << SDL_GetError() << std::endl;
        
        if (error_code ec = SDL_CreateWindowAndRenderer(900, 600, 0, &window, &renderer); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (error_code ec = SDL_SetRenderDrawColor(renderer, 12, 199, 166, 255); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        {
            auto r = std::make_unique<element>(renderer);
            rin.swap(r);
        }
        if (error_code ec = rin->set_texture("asset/rin.png"); ec < 0)
            std::cout << "Load rin image error" << std::endl;
    }
    
    ~app()
    {
        SDL_Quit();
    }

    void run()
    {
        if (not is_running)
        {
            emscripten_cancel_main_loop();
            return;
        }

        SDL_Event event;
        while (SDL_PollEvent (&event))
            handle_event(event);

        calculation();
        render();
    }

public:
    void handle_event(SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_MOUSEMOTION:
            std::cout << "mouse moved" << std::endl;
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    rin->active_state |= UP_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    rin->active_state ^= UP_PRESSED;
                break;

            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    rin->active_state |= DOWN_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    rin->active_state ^= DOWN_PRESSED;
                break;

            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    rin->active_state |= LEFT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    rin->active_state ^= LEFT_PRESSED;
                break;

            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    rin->active_state |= RIGHT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    rin->active_state ^= RIGHT_PRESSED;
                break;

            default:
                break;
            }
            std::cout << "key emit" << std::endl;
            break;
        }
        default:
            break;
        }
    }

    void calculation()
    {
//        rin->vx = rin->vy = 0;
        if (rin->active_state & UP_PRESSED)
            rin->vy = -50;
        if (rin->active_state & DOWN_PRESSED)
            rin->vy =  50;
        if (rin->active_state & LEFT_PRESSED)
            rin->vx = -50;
        if (rin->active_state & RIGHT_PRESSED)
            rin->vx =  50;

        rin->vx *= INV;
        rin->vy *= INV;

        rin->dest.x += rin->vx;
        rin->dest.y += rin->vy;
    }

    void render()
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy (renderer, rin->texture, NULL, &rin->dest);
        SDL_RenderPresent(renderer);
    }
};



} // namespace game
#endif // __APP_HPP__
