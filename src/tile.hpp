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
    enum class graph
    {
        none = 0,
        grass,
        brick,
        snow,
        water
    };
    graph graph_id = graph::none;

    enum class flag
    {
        none   = 0,
        solid  = 1,
        deadly = 1 << 1,
    };
    flag flag_id = flag::none;
    SDL_Rect src = {};

public:
    tile (graph g, flag f):
        graph_id{g},
        flag_id {f},
        src {
            .x = (utility::cast(g) - 1) * TILE_SIZE_PIXEL_INT,
            .y = 0,
            .w = TILE_SIZE_PIXEL_INT,
            .h = TILE_SIZE_PIXEL_INT
        } {}

    bool is_solid() { return utility::cast(flag_id) & utility::cast(flag::solid); }

    template <char c> inline constexpr static
    auto cast (int i)
    {
        if constexpr (c == 'g')
            return static_cast<tile::graph>(i);
        else if constexpr (c == 'f')
            return static_cast<tile::flag>(i);
        else
            return i;
    }
};

std::ostream & operator << (std::ostream & os, tile const & t)
{
    os << "[tile.flag]: "  << utility::cast(t.flag_id) << "\n"
       << "[tile.graph]: " << utility::cast(t.graph_id);
    return os;
}

} // namespace game
#endif // TILE_HPP_
