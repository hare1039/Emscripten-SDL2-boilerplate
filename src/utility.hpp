#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#pragma once
#include <random>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include "compile_time_utility.hpp"

namespace game::utility
{

constexpr inline
int get_array_pos(int x, int y, int width) { return y * width + x; }

template <typename E> constexpr inline
auto cast(E e) noexcept { return static_cast<std::underlying_type_t<E>>(e); }

template <typename T, typename ... Args> constexpr inline
T make_object(Args&& ... args) { T o(std::forward<Args>(args)...); return o; }

template <typename Comp, typename Bind, typename Max> inline
Bind&& normalize(Bind&& var, Max&& standard, Comp comp = Comp())
{
    if (comp(var, standard))
        var = standard;
    return var;
}

template <typename Bind, typename Max> inline
Bind&& trim(Bind&& var, Max&& standard)
{
    return normalize<std::greater_equal<Bind>>(std::forward<Bind>(var), std::forward<Max>(standard));
}

template <typename Bind, typename Max> inline
Bind&& trim_inv(Bind&& var, Max&& standard)
{
    return normalize<std::less_equal<Bind>>(std::forward<Bind>(var), std::forward<Max>(standard));
}

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

template<typename IntegerType,
         std::enable_if_t<std::is_integral<IntegerType>::value, int> = 0>
SDL_Color operator + (SDL_Color const & org, IntegerType bright)
{
    return SDL_Color {
        .r = org.r,
        .g = org.g,
        .b = org.b,
        .a = (org.a + bright) > std::numeric_limits<Uint8>::max() ?
              std::numeric_limits<Uint8>::max() : static_cast<Uint8>((org.a + bright))
    };
}

} // namespace game::utility

#endif // UTILITY_HPP_
