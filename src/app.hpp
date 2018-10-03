#ifndef __APP_HPP__
#define __APP_HPP__
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

#include "basic_headers.hpp"
#include "movable.hpp"

namespace game
{

class app
{
    bool should_continue = true;
    SDL_Window   * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    
    std::unique_ptr<movable> rin;
    std::unique_ptr<movable> yoshi;
public:
    app()
    {
        if (error_code ec = SDL_Init(SDL_INIT_VIDEO); ec < 0)
            std::cout << SDL_GetError() << std::endl;
        
        if (error_code ec = SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,
                                                        0,
                                                        &window, &renderer); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (error_code ec = SDL_SetRenderDrawColor(renderer, 12, 199, 166, 255); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        
        rin.reset (new movable(renderer));
        if (error_code ec = rin->set_texture("asset/rin.png"); ec < 0)
            std::cout << "Load rin image error" << std::endl;

        yoshi.reset (new movable(renderer));
        if (error_code ec = yoshi->set_texture("asset/yoshi.png", 8); ec < 0)
            std::cout << "Load yoshi image error" << std::endl;
    }
    
    ~app()
    {
        SDL_Quit();
    }

    void run()
    {
        if (not should_continue)
        {
            emscripten_cancel_main_loop();
            return;
        }

        SDL_Event event;
        while (SDL_PollEvent (&event))
            handle_event(event);

        calculate();
        render();
    }

public:
    void handle_event(SDL_Event& event)
    {
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [&] (element * e) { e->handle_event(event); });
    }

    void calculate()
    {
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [] (element * e) { e->calculate(); });
    }

    void render()
    {
        SDL_RenderClear(renderer);
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [] (element * e) { e->render(); });
        SDL_RenderPresent(renderer);
    }
};



} // namespace game
#endif // __APP_HPP__
