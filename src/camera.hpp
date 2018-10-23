#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#pragma once

#include <utility>
#include "basic_headers.hpp"
#include "element.hpp"
#include "enable_instance.hpp"

namespace game
{

class camera
{
public:
    enum class mode
    {
        top_left = 0,
        center
    } mode_id = mode::top_left;

    SDL_Rect pos = {0, 0, 0, 0}, *target = &pos;
public:

    void bind(SDL_Rect *t) { target = t; }
    void set(int x, int y) { if (target) { target->x = x; target->y = y;} }
    void shift(int x_shift, int y_shift) { if (target) {target->x += x_shift; target->y += y_shift;} }
    std::pair<pixel, pixel> get_pos()
    {
        std::pair<pixel, pixel> p {target->x, target->y};
        switch (mode_id)
        {
        case mode::top_left:
            break;
        case mode::center:
            p.first  -= (WINDOW_WIDTH_PIXEL / 2 - target->w / 2);
            p.second -= (WINDOW_HEIGHT_PIXEL / 2 - target->h / 2);
            break;
        }
        return p;
    }
};

} // namespace game
#endif // CAMERA_HPP_
