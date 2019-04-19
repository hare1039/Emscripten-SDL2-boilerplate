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
    front_page(SDL_Renderer * r): theme{r, "./asset/theme/front.toml"} {}

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
        switch (key)
        {
            using namespace element_types;
        case SDLK_UP:
            option_group<option>::registry()["game-selection"].lock()->
                select(*dynamic_cast<option *>(elements["option-2-player"].get()));
            break;

        case SDLK_DOWN:
            option_group<option>::registry()["game-selection"].lock()->
                select(*dynamic_cast<option *>(elements["option-online"].get()));
            break;

        case SDLK_RETURN:
            if (dynamic_cast<option *>(elements["option-2-player"].get())->is_selected())
                next_theme = std::make_unique<court>(renderer);
            break;
        default:
            break;
        }
    }
};

} // namespace game

#endif // THEMES_FRONT_PAGE_HPP_
