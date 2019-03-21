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

    void on_key_down(SDL_Keycode const & key, Uint16 const & mod) override
    {
        elements["player1"]->on_key_down(key, mod);
        elements["player2"]->on_key_down(key, mod);
        switch (key)
        {
        case SDLK_DOWN:
            for (auto && p : elements)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme = std::make_unique<stage>(renderer, "./asset/theme/01.toml");
            break;

        case SDLK_SPACE:
//            elements["rin"]->dest.x += 100;
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &mod) override
    {
        elements["player1"]->on_key_up(key, mod);
        elements["player2"]->on_key_up(key, mod);
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
