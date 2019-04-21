#ifndef ELEMENTS_FLOATING_HPP_
#define ELEMENTS_FLOATING_HPP_

#pragma once

#include <string_view>
#include "../basic_headers.hpp"
#include "../element.hpp"
#include "../camera.hpp"

namespace game::element_types
{

class floating : public element
{
public:
    floating(SDL_Renderer *r,
             std::string_view name,
             cache_container<std::string, element> &a,
             camera &c,
             std::unique_ptr<fps>* game_fps): element{r, name, a, c, game_fps} {}

    void render() override final
    {
        SDL_Rect && pos {state_.dest_.to_sdl()};
        SDL_RenderCopy (renderer_, texture_.get(), &src_, &pos);
    }
};

} // namespace

#endif // ELEMENTS_FLOATING_HPP_
