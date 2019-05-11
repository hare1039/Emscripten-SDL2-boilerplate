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
    pixel camera_x_ = 0, camera_y_ = 0;
public:
    front_page(SDL_Renderer * r, std::unique_ptr<fps>* gfps): theme {r, gfps, "./asset/theme/front.toml"} {}

    void calculate() override final
    {
        // 1080: front-page background map width

        double shift = (*game_fps)->speed_factor() * 8;
        if (camera_x_ + WINDOW_WIDTH_PIXEL >= 1080 - shift)
            camera_x_ = camera_y_ = 0;
        else
        {
            camera_x_ += shift;
            camera_y_ += shift;
        }
        theme_camera->set(camera_x_, camera_y_);
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
            if (dynamic_cast<option *>(elements["option-local"].get())->is_selected())
                next_theme = std::make_unique<court>(renderer, game_fps);
            else if (dynamic_cast<option *>(elements["option-online"].get())->is_selected())
                next_theme = std::make_unique<stage>(renderer, game_fps, "./asset/theme/01.toml");
            break;
        default:
            break;
        }
    }
};

} // namespace game

#endif // THEMES_FRONT_PAGE_HPP_
