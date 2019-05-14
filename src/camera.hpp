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
    };
    mode mode_ = mode::top_left;

    rect<pixel> pos_ {0, 0, 0, 0}, *target_ = &pos_;
public:

    void bind(rect<pixel> *t) { target_ = t; }
    void set (int x, int y)   { if (target_) { target_->x = x; target_->y = y;} }
    void shift(int x_shift, int y_shift) { if (target_) {target_->x += x_shift; target_->y += y_shift;} }
    std::pair<pixel, pixel> get_pos()
    {
        std::pair<pixel, pixel> p {target_->x, target_->y};
        switch (mode_)
        {
        case mode::top_left:
            break;
        case mode::center:
            p.first  -= (WINDOW_WIDTH_PIXEL / 2 - target_->w / 2);
            p.second -= (WINDOW_HEIGHT_PIXEL / 2 - target_->h / 2);
            break;
        }
        return p;
    }
};

} // namespace game
#endif // CAMERA_HPP_
