#ifndef APP_HPP_
#define APP_HPP_
#pragma once

#include "basic_headers.hpp"
#include "theme.hpp"
namespace game
{

class app
{
    bool should_continue = true;
    SDL_Window_ptr   window{nullptr, &SDL_DestroyWindow};
    SDL_Renderer_ptr renderer{nullptr, &SDL_DestroyRenderer};
    std::unique_ptr<fps>   game_fps;
    std::unique_ptr<theme> thm;

public:
    app():
        game_fps {std::make_unique<fps>()}
    {
        if (error_code ec = SDL_Init(SDL_INIT_VIDEO); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        SDL_Window   * wind = nullptr;
        SDL_Renderer * rend = nullptr;
        if (error_code ec = SDL_CreateWindowAndRenderer(WINDOW_WIDTH_PIXEL, WINDOW_HEIGHT_PIXEL,
                                                        0,
                                                        &wind, &rend); ec < 0)
            std::cout << SDL_GetError() << std::endl;
        window.reset(wind);
        renderer.reset(rend);

        if (error_code ec = SDL_SetRenderDrawColor(renderer.get(), 12, 199, 166, 255); ec < 0)
            std::cout << SDL_GetError() << std::endl;

        thm = std::make_unique<theme>(renderer.get(), "./asset/theme/01.toml");
    }

    ~app() { SDL_Quit(); }

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
    void handle_event(SDL_Event& event) { thm->on_event(event); }

    void calculate()
    {
        game_fps->calculate();
        thm->calculate();
    }

    void render()
    {
        SDL_RenderClear(renderer.get());
        thm->render();
        SDL_RenderPresent(renderer.get());
    }
};

} // namespace game
#endif // APP_HPP_
