#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#pragma once

#include <utility>
#include "basic_headers.hpp"
#include "enable_instance.hpp"

namespace game
{

class camera : public enable_instance<camera>
{
public:
    class pos_pair
    {
    public:
        pixel x = 0;
        pixel y = 0;

        pos_pair& operator += (const pos_pair & p) { x += p.x; y += p.y; return *this; }
        pos_pair& operator =  (const pos_pair & p) { x =  p.x; y  = p.y; return *this; }
    } pos;

    enum class camera_mode
    {
        top_left = 0,
        center
    } mode = camera_mode::top_left;
public:
    
    void shift(pos_pair p) { pos += p; }
    pos_pair get_pos()
    {
        pos_pair p = pos;
        switch (mode)
        {
        case camera_mode::top_left:
            break;
        case camera_mode::center:
            p.x -= (WINDOW_WIDTH_PIXEL / 2);
            p.y -= (WINDOW_HEIGHT_PIXEL / 2);
            break;
        }
        return p;
    }
};

} // namespace game
#endif // CAMERA_HPP_
