#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#pragma once

#include "basic_headers.hpp"

namespace game::utility
{

constexpr inline static
int get_array_pos(int x, int y, int width) { return y * width + x; }

}


#endif // __UTILITY_HPP__
