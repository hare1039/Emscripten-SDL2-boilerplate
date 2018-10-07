#ifndef APP_HPP_
#define APP_HPP_
#pragma once

#include "basic_headers.hpp"
#include "movable.hpp"
#include "camera.hpp"
#include "area.hpp"

namespace game
{

class app
{
    bool should_continue = true;
    SDL_Window   * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    
    std::unique_ptr<movable> rin;
    std::unique_ptr<movable> yoshi;
    std::unique_ptr<area>    a;
    std::unique_ptr<camera>  cam;
public:
    app()
    {
        if (error_code ec = SDL_Init(SDL_INIT_VIDEO); ec < 0)
            std::cout << SDL_GetError() << std::endl;
        
        if (error_code ec = SDL_CreateWindowAndRenderer(WINDOW_WIDTH_PIXEL, WINDOW_HEIGHT_PIXEL,
                                                        0,
                                                        &window, &renderer); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (error_code ec = SDL_SetRenderDrawColor(renderer, 12, 199, 166, 255); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        
        rin.reset (new movable(renderer));
        if (error_code ec = rin->set_texture("asset/pic/rin.png"); ec < 0)
            std::cout << "Load rin image error" << std::endl;

        yoshi.reset (new movable(renderer));
        if (error_code ec = yoshi->set_texture("asset/pic/yoshi.png", 8); ec < 0)
            std::cout << "Load yoshi image error" << std::endl;
        a.reset(new area(renderer, "./asset/map/00.area"));
        cam.reset(new camera);
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
        move_camera(event);
    }

    void calculate()
    {
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [] (element * e) { e->calculate(); });
    }

    void render()
    {
        SDL_RenderClear(renderer);
        auto [x, y] = cam->get_pos();
        a->render (x, y);
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [] (element * e) { e->render(); });
        SDL_RenderPresent(renderer);
    }

    void move_camera (SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                cam->shift(camera::pos_pair{ 0,  20});
                break;
            case SDLK_DOWN:
                cam->shift(camera::pos_pair{ 0, -20});
                break;
            case SDLK_LEFT:
                cam->shift(camera::pos_pair{ 20,  0});
                break;
            case SDLK_RIGHT:
                cam->shift(camera::pos_pair{-20,  0});
                break;
            }
            break;
        }
        }
    }
};

} // namespace game
#endif // APP_HPP_
