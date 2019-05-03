#ifndef ELEMENTS_PLAYER_HPP_
#define ELEMENTS_PLAYER_HPP_

#include <cassert>
#include "../keyboard_state.hpp"
#include "movable.hpp"

namespace game::elements::types
{

class player : public movable
{
    enum class key {up, down, left, right, smash};
    keyboard_state<key> keyboard_;
    enum class smash {right = 1, none = 0, left = -1};
    smash smash_;
public:
    player(SDL_Renderer *r,
           std::string_view name,
           cache_container<std::string, element> &a,
           camera &c,
           std::unique_ptr<fps>* game_fps): movable {r, name, a, c, game_fps}
    {
        type_ = type::player; // not suggested in stackoverflow, TODO list!
        hardness_ = hardness::stone;
        ignore_types_ = type::ball;
        collides_types_ = type::fence;
    }

    virtual
    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        movable::build_from_toml(table);
        assert(table->contains("keyup")   and
               table->contains("keydown") and
               table->contains("keyleft") and
               table->contains("keyright")and
               table->contains("keysmash")and
               table->contains("smash"));

        keyboard_.watch(*table->get_as<SDL_Keycode>("keyup"),
                        *table->get_as<SDL_Keycode>("keydown"),
                        *table->get_as<SDL_Keycode>("keyleft"),
                        *table->get_as<SDL_Keycode>("keyright"),
                        *table->get_as<SDL_Keycode>("keysmash"));

        keyboard_.bind_bool(key::left,  move_left_,
                            key::right, move_right_);

        smash_ =
            *table->get_as<std::string>("smash") == "right"? smash::right :
            *table->get_as<std::string>("smash") == "left" ? smash::left  : smash::none;
    }

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override
    {
        keyboard_.on_key_down(key);
        if (keyboard_.is_pressing(key::up))
            jump();
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        keyboard_.on_key_up(key);
    }

    void on_collision (element & e) override
    {
        if (state_.dest_.y + col_h() < e.state_.dest_.y + e.col_offset_)
            jumpable_ = true;

        if (e.type_ == type::ball)
        {
            element &ball = e;
            if (keyboard_.is_pressing(key::smash))
            {
                ball.state_.speed_x_ = static_cast<int>(smash_) * 30;
                if (keyboard_.is_pressing(key::up))
                    ball.state_.speed_y_ = -70;
                if (keyboard_.is_pressing(key::down))
                    ball.state_.speed_y_ = 70;

                if ((not keyboard_.is_pressing(key::up)) and
                    (not keyboard_.is_pressing(key::down)))
                    ball.state_.speed_y_ = 0;

                if (keyboard_.is_pressing(key::right) or
                    keyboard_.is_pressing(key::left))
                    ball.state_.speed_x_ = static_cast<int>(smash_) * 70;
            }
            else
            {
                if (mid_point().x < ball.mid_point().x)
                    ball.state_.speed_x_ =   state_.old_speed_x_ + 30;
                else
                    ball.state_.speed_x_ = -(state_.old_speed_x_ + 30);

                ball.state_.speed_y_ = -(state_.old_speed_y_ + 70);
            }
        }
    }
};

} // namespace game::elements::types

#endif // ELEMENTS_PLAYER_HPP_
