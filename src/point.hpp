#ifndef POINT_HPP_
#define POINT_HPP_

#include "utility.hpp"

template <typename Point,
          std::enable_if_t<game::utility::type_traits::is_point<Point>::value, int> = 0>
std::ostream& operator << (std::ostream &os, Point const & r)
{
    return os << "{x: " << r.x << "\n"
              << " y: " << r.y << "}\n";
}

namespace game
{

template <typename Point,
          std::enable_if_t<game::utility::type_traits::is_point<Point>::value, int> = 0>
std::ostream& operator << (std::ostream &os, Point const & r)
{
    return ::operator << (os, r);
}

template <typename T = pixel>
struct point
{
    T x, y;
    operator SDL_Point() const
    {
        return SDL_Point {
            .x = static_cast<int>(x),
            .y = static_cast<int>(y)
        };
    }

    SDL_Point to_sdl() const
    {
        return static_cast<SDL_Point>(*this);
    }
};

} // namespace game

#endif // POINT_HPP_
