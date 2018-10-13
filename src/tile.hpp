#ifndef TILE_HPP_
#define TILE_HPP_
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
        grass,
        brick,
        snow,
        water
    };

    int  id = 0;
    type type_id = type::none;
    SDL_Rect src = {};

public:
    tile (int i, type t):
        id{i},
        type_id{t},
        src {
            .x = (cast(t) - 1) * TILE_SIZE_PIXEL, //(cast(t) % (surface->w / TILE_SIZE_PIXEL)) * TILE_SIZE_PIXEL,
            .y = 0, //(cast(t) / (surface->w / TILE_SIZE_PIXEL)) * TILE_SIZE_PIXEL,
            .h = TILE_SIZE_PIXEL,
            .w = TILE_SIZE_PIXEL
        } {}

    bool is_solid() { return type_id == tile::type::water; }

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
#endif // TILE_HPP_
