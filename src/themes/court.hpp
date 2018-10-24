#ifndef THEMES_COURT_HPP_
#define THEMES_COURT_HPP_

#pragma once

#include "../basic_headers.hpp"
#include "../theme.hpp"
#include "stage.hpp"
namespace game::theme_types
{

class court : public theme
{
public:
    court(SDL_Renderer * r): theme{r, "./asset/theme/court.toml"} {}

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override final
    {
        switch (key)
        {
        case SDLK_UP:
            elements["yoshi"]->jump();
            break;

        case SDLK_DOWN:
            for (auto && p : elements)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme = std::make_unique<stage>(renderer, "./asset/theme/01.toml");
            break;

        case SDLK_LEFT:
            elements["yoshi"]->move_left = true;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = true;
            break;

        case SDLK_SPACE:
            elements["yoshi"]->jump();
            elements["rin"]->dest.x += 100;
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        switch (key)
        {
        case SDLK_LEFT:
            elements["yoshi"]->move_left = false;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = false;
            break;

        default:
            break;
        }
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
