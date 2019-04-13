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
    enum player {player1, player2};
    std::array<rect<>, 2> rect_{};
    std::array<unsigned int, 2> score_{};
public:
    court(SDL_Renderer * r): theme{r, "./asset/theme/court.toml"}
    {
        rect_.at(player1) = elements["player1"]->state_.dest_;
        rect_.at(player2) = elements["player2"]->state_.dest_;
    }

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
        if (score_.at(player1) != p1floor->count())
        {
            score_.at(player1) = p1floor->count();
            auto sb = dynamic_cast<element_types::text *>(elements["right-scoreboard"].get());
            sb->update_text(std::to_string(score_.at(player1)));
            serve(player1);
        }

        auto p2floor = dynamic_cast<element_types::score_counter *>(elements["left-floor"].get());
        if (score_.at(player2) != p2floor->count())
        {
            score_.at(player2) = p2floor->count();
            auto sb = dynamic_cast<element_types::text *>(elements["left-scoreboard"].get());
            sb->update_text(std::to_string(score_.at(player2)));
            serve(player2);
        }
    }

private:
    void serve (player p)
    {
        switch(p)
        {
        case player1:
            elements["ball"]->state_.dest_.x  = 100;
            elements["ball"]->state_.dest_.y  = 200;
            elements["ball"]->state_.speed_x_ = 0;
            elements["ball"]->state_.speed_y_ = 0;
            break;

        case player2:
            elements["ball"]->state_.dest_.x  = 800;
            elements["ball"]->state_.dest_.y  = 200;
            elements["ball"]->state_.speed_x_ = 0;
            elements["ball"]->state_.speed_y_ = 0;
            break;
        }
        reset_theme();
    }

    void reset_theme()
    {
        elements["player1"]->state_.dest_ = rect_.at(player1);
        elements["player2"]->state_.dest_ = rect_.at(player2);
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
