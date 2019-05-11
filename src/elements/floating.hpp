#ifndef ELEMENTS_FLOATING_HPP_
#define ELEMENTS_FLOATING_HPP_

#pragma once

#include <string_view>
#include "../basic_headers.hpp"
#include "../element.hpp"
#include "../camera.hpp"

namespace game::elements::types
{

class floating : public element
{
public:
    floating(std::string_view name,
             cache_container<std::string, element> &a,
             SDL_Renderer *r,
             camera &c,
             std::unique_ptr<fps>* game_fps): element{name, a, r, c, game_fps} {}

    void render() override final
    {
        SDL_Rect && pos {state_.dest_.to_sdl()};
        SDL_RenderCopy (renderer_, texture_.get(), &src_, &pos);
    }
};

} // namespace game::elements::types

#endif // ELEMENTS_FLOATING_HPP_
