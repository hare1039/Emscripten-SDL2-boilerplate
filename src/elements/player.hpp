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
           std::unordered_map<std::string, std::unique_ptr<element>> &a,
           camera &c): movable {r, name, a, c} {}

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
            move_left = true;

        else if (key == keyright_)
            move_right = true;
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        if (key == keyleft_)
            move_left = false;

        else if (key == keyright_)
            move_right = false;
    }
};

} // namespace game::element_types

#endif // ELEMENTS_PLAYER_HPP_
