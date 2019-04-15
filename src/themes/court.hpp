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
    enum player {player1 /* right player */,
                 player2 /* left  player */, player_list_end};
    std::array<rect<>, player_list_end> rect_{};
    std::array<unsigned int, player_list_end> score_{};
public:
    court(SDL_Renderer * r): theme {r, "./asset/theme/court.toml"}
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
            fps::instance()->pause();
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &mod) override
    {
        elements["player1"]->on_key_up(key, mod);
        elements["player2"]->on_key_up(key, mod);
        switch (key)
        {
        case SDLK_SPACE:
            fps::instance()->resume();
            break;

        default:
            break;
        }
    }

    void calculate() override
    {
        theme::calculate();
        std::array<element_types::score_counter *, player_list_end> score_counter {{
            dynamic_cast<element_types::score_counter *>(elements["left-floor"].get()),
            dynamic_cast<element_types::score_counter *>(elements["right-floor"].get())
        }};
        player winner = player_list_end;
        if (score_.at(player1) != score_counter.at(player1)->count())
            winner = player1;

        if (score_.at(player2) != score_counter.at(player2)->count())
        {
            // if score of player2 also increase, make sure which one are the real winner
            if (winner == player1)
            {
                if (elements["ball"]->mid_point().x > elements["right-floor"]->state_.dest_.x)
                {
                    winner = player2;
                    score_counter.at(player1)->count() = score_.at(player1);
                }
                else
                {
                    winner = player1;
                    score_counter.at(player2)->count() = score_.at(player2);
                }
            }
            else
            {
                winner = player2;
            }
        }

        if (winner != player_list_end)
        {
            score_.at(winner) = score_counter.at(winner)->count();
            dynamic_cast<element_types::text *>(
                (winner == player1)?
                    elements["right-scoreboard"].get():
                    elements["left-scoreboard"] .get())
                ->update_text(std::to_string(score_.at(winner)));
            serve(winner);
        }
    }

private:
    void serve (player p)
    {
        switch(p)
        {
        case player1:
            elements["ball"]->state_.dest_.x  = 800;
            elements["ball"]->state_.dest_.y  = 200;
            elements["ball"]->state_.speed_x_ = 0;
            elements["ball"]->state_.speed_y_ = 0;
            break;

        case player2:
            elements["ball"]->state_.dest_.x  = 100;
            elements["ball"]->state_.dest_.y  = 200;
            elements["ball"]->state_.speed_x_ = 0;
            elements["ball"]->state_.speed_y_ = 0;
            break;

        default:
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
