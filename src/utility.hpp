#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#pragma once

#include <iostream>
#include "basic_headers.hpp"

namespace game::utility
{

constexpr inline
int get_array_pos(int x, int y, int width) { return y * width + x; }

template <typename E> constexpr
auto cast(E e) noexcept { return static_cast<std::underlying_type_t<E>>(e); }

}

std::ostream& operator << (std::ostream &os, SDL_Rect const & r)
{
    os << "{x: " << r.x << "\n"
       << " y: " << r.y << "\n"
       << " w: " << r.w << "\n"
       << " h: " << r.h << "}";
    return os;
}

#endif // UTILITY_HPP_
