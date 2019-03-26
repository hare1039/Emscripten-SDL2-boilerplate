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
             std::unordered_map<std::string, std::unique_ptr<element>> &a,
             camera &c): element{r, name, a, c} {}

    void render() override final
    {
        SDL_Rect && pos {dest.to_sdl()};
        SDL_RenderCopy (renderer, texture.get(), &src, &pos);
    }
};

} // namespace

#endif // ELEMENTS_FLOATING_HPP_
