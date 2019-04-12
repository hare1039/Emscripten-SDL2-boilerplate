#ifndef THEMES_COURT_HPP_
#define THEMES_COURT_HPP_

#pragma once

#include <array>
#include "../basic_headers.hpp"
#include "../theme.hpp"
#include "stage.hpp"
namespace game::theme_types
{

class court : public theme
{
    enum {player1, player2};
    std::array<unsigned int, 2> score{};
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
            next_theme = std::make_unique<court>(renderer);
            break;

        case SDLK_SPACE:
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

    void calculate() override
    {
        theme::calculate();
        auto p1floor = dynamic_cast<element_types::score_counter *>(elements["right-floor"].get());
        if (score.at(player1) != p1floor->count())
        {
            score.at(player1) = p1floor->count();
            auto sb = dynamic_cast<element_types::text *>(elements["right-scoreboard"].get());
            sb->update_text(std::to_string(score.at(player1)));
        }

        auto p2floor = dynamic_cast<element_types::score_counter *>(elements["left-floor"].get());
        if (score.at(player2) != p2floor->count())
        {
            score.at(player2) = p2floor->count();
            auto sb = dynamic_cast<element_types::text *>(elements["left-scoreboard"].get());
            sb->update_text(std::to_string(score.at(player2)));
        }
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
