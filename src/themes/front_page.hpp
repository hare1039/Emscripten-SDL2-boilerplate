#ifndef THEMES_FRONT_PAGE_HPP_
#define THEMES_FRONT_PAGE_HPP_

#pragma once

#include "../basic_headers.hpp"
#include "../theme.hpp"
#include "court.hpp"
#include "stage.hpp"
namespace game::theme_types
{

class front_page : public theme
{
public:
    front_page(SDL_Renderer * r, std::unique_ptr<fps>* gfps): theme {r, gfps, "./asset/theme/front.toml"} {}

    void calculate() override final
    {
        auto [x, y] = theme_camera->get_pos();
        // 1080 here is a magic number. It is used for making front-page background loop
        if (x + WINDOW_WIDTH_PIXEL >= 1080 - 2)
            theme_camera->set(0, 0);
        else
            theme_camera->shift(2, 2);
        theme::calculate();
    }

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override final
    {
        using namespace elements::types;
        switch (key)
        {
        case SDLK_UP:
            option::group::select_prev("game-selection");
            break;

        case SDLK_DOWN:
            option::group::select_next("game-selection");
            break;

        case SDLK_RETURN:
            if (dynamic_cast<option *>(elements["option-2-player"].get())->is_selected())
                next_theme = std::make_unique<court>(renderer, game_fps);
            break;
        default:
            break;
        }
    }
};

} // namespace game

#endif // THEMES_FRONT_PAGE_HPP_
