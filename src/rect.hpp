#ifndef RECT_HPP_
#define RECT_HPP_

#include "utility.hpp"

template <typename Rect,
          std::enable_if_t<game::utility::type_traits::is_rectangle<Rect>::value, int> = 0>
std::ostream& operator << (std::ostream &os, Rect const & r)
{
    return os << "{x: " << r.x << "\n"
              << " y: " << r.y << "\n"
              << " w: " << r.w << "\n"
              << " h: " << r.h << "}";
}

namespace game
{

template <typename Rect,
          std::enable_if_t<game::utility::type_traits::is_rectangle<Rect>::value, int> = 0>
std::ostream& operator << (std::ostream &os, Rect const & r)
{
    return ::operator <<(os, r);
}

template <typename T = pixel>
struct rect
{
    T x, y, w, h;
    operator SDL_Rect() const
    {
        return SDL_Rect {
            .x = static_cast<int>(x),
            .y = static_cast<int>(y),
            .w = static_cast<int>(w),
            .h = static_cast<int>(h)
        };
    }

    SDL_Rect to_sdl() const
    {
        return static_cast<SDL_Rect>(*this);
    }
};

} // namespace game

#endif // RECT_HPP_
