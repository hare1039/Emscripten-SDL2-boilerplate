#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#pragma once

#include "basic_headers.hpp"

namespace game::utility
{

constexpr inline
int get_array_pos(int x, int y, int width) { return y * width + x; }

}


#endif // UTILITY_HPP_
