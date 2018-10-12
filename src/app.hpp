#ifndef APP_HPP_
#define APP_HPP_
#pragma once

#include "basic_headers.hpp"
#include "movable.hpp"
#include "camera.hpp"
#include "area.hpp"
#include "fps.hpp"
namespace game
{

class app
{
    bool should_continue = true;
    SDL_Window   * window = nullptr;
    SDL_Renderer * renderer = nullptr;
    
    std::unique_ptr<movable> yoshi;
    std::unique_ptr<area>    a;
    std::unique_ptr<camera>  cam;
    std::unique_ptr<fps>     f;
public:
    app():
        cam{std::make_unique<camera>()},
        f{std::make_unique<fps>()}
    {
        if (error_code ec = SDL_Init(SDL_INIT_VIDEO); ec < 0)
            std::cout << SDL_GetError() << std::endl;
        
        if (error_code ec = SDL_CreateWindowAndRenderer(WINDOW_WIDTH_PIXEL, WINDOW_HEIGHT_PIXEL,
                                                        0,
                                                        &window, &renderer); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        if (error_code ec = SDL_SetRenderDrawColor(renderer, 12, 199, 166, 255); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        yoshi = std::make_unique<movable> (renderer);
        if (error_code ec = yoshi->set_texture("./asset/pic/yoshi.png", 64, 64, animation::rotate_type::circle); ec < 0)
            std::cout << "Load yoshi image error" << std::endl;
        yoshi->dest.x = 450;
        yoshi->dest.y = 450;
        cam->bind(&yoshi->dest.x, &yoshi->dest.y);
        cam->mode_id = camera::mode::center;
        a = std::make_unique<area>(renderer, "./asset/map/00.area");
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
        f->calculate();
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
        return ;
        switch (event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                yoshi->move( 0, 20);
                break;
            case SDLK_DOWN:
                yoshi->move( 0, -20);
                break;
            case SDLK_LEFT:
                yoshi->move( 20,  0);
                break;
            case SDLK_RIGHT:
                yoshi->move(-20,  0);
                break;
            }
            break;
        }
        }
    }
};

} // namespace game
#endif // APP_HPP_
