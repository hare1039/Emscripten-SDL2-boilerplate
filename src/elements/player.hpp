#ifndef ELEMENTS_PLAYER_HPP_
#define ELEMENTS_PLAYER_HPP_

#include <cassert>
#include "movable.hpp"

namespace game::element_types
{

class player : public movable
{
    SDL_Keycode keyup_, keydown_, keyleft_, keyright_;
public:
    player(SDL_Renderer *r,
           std::string_view name,
           cache_container<std::string, element> &a,
           camera &c,
           std::unique_ptr<fps>* game_fps): movable {r, name, a, c, game_fps}
    {
        type_ = type::player; // not suggested in stackoverflow, TODO list!
        hardness_ = hardness::stone;
    }

    virtual
    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        movable::build_from_toml(table);
        assert(table->contains("keyup")   and
               table->contains("keydown") and
               table->contains("keyleft") and
               table->contains("keyright"));

        keyup_    = *table->get_as<SDL_Keycode>("keyup");
        keydown_  = *table->get_as<SDL_Keycode>("keydown");
        keyleft_  = *table->get_as<SDL_Keycode>("keyleft");
        keyright_ = *table->get_as<SDL_Keycode>("keyright");
    }

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override
    {
        if (key == keyup_)
            jump();

        else if (key == keyleft_)
            move_left_ = true;

        else if (key == keyright_)
            move_right_ = true;
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        if (key == keyleft_)
            move_left_ = false;

        else if (key == keyright_)
            move_right_ = false;
    }

    void on_collision (element & e) override
    {
        if (state_.dest_.y + col_h() < e.state_.dest_.y + e.col_offset_)
            jumpable_ = true;

        if (e.type_ == type::ball)
        {
            element &ball = e;
            if (mid_point().x < ball.mid_point().x)
                ball.state_.speed_x_ =   state_.old_speed_x_ + 30;
            else
                ball.state_.speed_x_ = -(state_.old_speed_x_ + 30);

            ball.state_.speed_y_ = -(state_.old_speed_y_ + 70);
        }
    }
};

} // namespace game::element_types

#endif // ELEMENTS_PLAYER_HPP_
