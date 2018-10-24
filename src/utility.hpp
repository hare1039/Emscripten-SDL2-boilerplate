#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#pragma once
#include <random>
#include <string>
#include <algorithm>
#include <iostream>
#include "basic_headers.hpp"

namespace game::utility
{

constexpr inline
int get_array_pos(int x, int y, int width) { return y * width + x; }

template <typename E> constexpr inline
auto cast(E e) noexcept { return static_cast<std::underlying_type_t<E>>(e); }

std::string random_string(std::string::size_type length)
{
    std::string str(length, 0);
    std::generate_n(str.begin(), length, []() -> char {
        static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

        thread_local static std::mt19937 rg{std::random_device{}()};
        thread_local static std::uniform_int_distribution<std::string::size_type> pick{0, sizeof(charset) - 2};
        return charset [pick(rg)];
    });
    return str;
}

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
