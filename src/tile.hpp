#ifndef __TILE_HPP__
#define __TILE_HPP__
#pragma once

#include <iostream>
#include "basic_headers.hpp"

namespace game
{

class tile
{
public:
    enum class type
    {
        none = 0,
        land,
        water
    };

    int  id = 0;
    type type_id = type::none;
    SDL_Rect src = {};

public:
    tile (int i, type t, SDL_Surface * surface):
        id{i},
        type_id{t},
        src {
            .x = (cast(t) % (surface->w / TILE_SIZE_PIXEL)) * TILE_SIZE_PIXEL,
            .y = (cast(t) / (surface->w / TILE_SIZE_PIXEL)) * TILE_SIZE_PIXEL,
            .h = TILE_SIZE_PIXEL,
            .w = TILE_SIZE_PIXEL
        } {}
    
    inline constexpr static
    tile::type cast (int i) { return static_cast<tile::type>(i); }

    inline constexpr static
    int cast (tile::type t) { return static_cast<int>(t); }
};

std::ostream & operator << (std::ostream & os, tile const & t)
{
    os << "[tile.id]: " << t.id << "\n"
       << "[tile.type_id]: " << tile::cast(t.type_id);
    return os;
}

} // namespace game
#endif // __TILE_HPP__
