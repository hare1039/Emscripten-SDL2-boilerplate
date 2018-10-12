#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#pragma once

#include <utility>
#include "basic_headers.hpp"
#include "element.hpp"
#include "enable_instance.hpp"

namespace game
{

class camera : public enable_instance<camera>
{
public:
    enum class mode
    {
        top_left = 0,
        center
    } mode_id = mode::top_left;

    int *x = nullptr;
    int *y = nullptr;
public:

    void bind(int *target_x, int *target_y)
    {
        x = target_x;
        y = target_y;
    }
    
    void shift(int x_shift, int y_shift) { if (not x and not y) {*x += x_shift; *y += y_shift;} }
    std::pair<pixel, pixel> get_pos()
    {
        std::pair<pixel, pixel> p {*x, *y};
        switch (mode_id)
        {
        case mode::top_left:
            break;
        case mode::center:
            p.first  -= (WINDOW_WIDTH_PIXEL / 2);
            p.second -= (WINDOW_HEIGHT_PIXEL / 2);
            break;
        }
        return p;
    }
};

} // namespace game
#endif // CAMERA_HPP_
