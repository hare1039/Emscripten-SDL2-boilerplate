#ifndef THEMES_COURT_HPP_
#define THEMES_COURT_HPP_

#pragma once

#include "../basic_headers.hpp"
#include "../theme.hpp"

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
            theme_camera->shift(0, -30);
            break;
        case SDLK_DOWN:
            theme_camera->shift(0, 30);
            break;
        case SDLK_LEFT:
            theme_camera->shift(-30, 0);
            break;
        case SDLK_RIGHT:
            theme_camera->shift(30, 0);
            break;
        default:
            break;
        }
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
