#ifndef THEMES_COURT_HPP_
#define THEMES_COURT_HPP_

#pragma once

#include <array>
#include "../basic_headers.hpp"
#include "../theme.hpp"
#include "../element_effects/collection.hpp"
#include "stage.hpp"

namespace game::theme_types
{

class court : public theme
{
    enum player {player1 /* right player */,
                 player2 /* left  player */, player_list_end};
    bool game_ends_ = false;
    std::array<rect<>, player_list_end> rect_{};
    std::array<unsigned int, player_list_end> score_{};
    unsigned int max_score_ = 1;

public:
    court(SDL_Renderer * r, std::unique_ptr<fps>* gfps): theme {r, gfps, "./asset/theme/court.toml"}
    {
        rect_.at(player1) = elements["player1"]->state_.dest_;
        rect_.at(player2) = elements["player2"]->state_.dest_;
        reset_theme();
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
            next_theme = std::make_unique<court>(renderer, game_fps);
            break;

        case SDLK_SPACE:
            (*game_fps)->pause();
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
            (*game_fps)->resume();
            break;

        default:
            break;
        }
    }

    void calculate() override
    {
        theme::calculate();
        if (game_ends_)
            return;

        std::array<elements::types::score_counter *, player_list_end> score_counter {{
            dynamic_cast<elements::types::score_counter *>(elements["left-floor"].get()),
            dynamic_cast<elements::types::score_counter *>(elements["right-floor"].get())
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
            dynamic_cast<elements::types::text *>(
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
        if (score_.at(player1) >= max_score_)
        {
            end_game(player1);
            return;
        }
        else if (score_.at(player2) >= max_score_)
        {
            end_game(player2);
            return;
        }

        elements["player1"]->state_.dest_ = rect_.at(player1);
        elements["player2"]->state_.dest_ = rect_.at(player2);

        using namespace std::literals;
        using namespace elements::effects;

        enable_animation(*elements["ready-text"]);
        animation.set(1500ms,
                      compose(500ms, [this, fade = make<fade>(500ms)] () mutable {
                                         fade(*elements["ready-text"]);
                                     },
                              500ms, [this, fade = make<fade>(500ms)] () mutable {
                                         fade(*elements["ready-text"]);
                                     },
                              500ms, [this, fade = make<fade>(500ms)] () mutable {
                                         fade(*elements["ready-text"]);
                                     }),
                      default_resume());
        animation.start();
    }

    void end_game(player winner)
    {
        using namespace std::literals;
        using namespace elements::effects;
        enable_animation(*elements["player1"], *elements["player2"]);

        elements["game-set-text"]->set_alpha(255);
        animation.set(4000ms,
                      [this, winner,
                       fade    = make<fade>(4000ms),
                       amplify = make<amplify>(4000ms, 7.0)] () mutable {
                          switch(winner)
                          {
                          case player1:
                              fade(*elements["player2"]);
                              amplify(*elements["player2"]);
                              break;
                          case player2:
                              fade(*elements["player1"]);
                              amplify(*elements["player1"]);
                              break;
                          default: break;
                          }
                      },
                      [this] { next_theme = std::make_unique<theme_types::stage>(
                                            renderer, game_fps, "./asset/theme/01.toml");});
        animation.start();
        game_ends_ = true;
    }
};

} // game::theme_types

#endif // THEMES_COURT_HPP_
