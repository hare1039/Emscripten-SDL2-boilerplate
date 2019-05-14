#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#pragma once

#include <chrono>
#include "basic_headers.hpp"

namespace game
{

namespace {
    using namespace std::chrono_literals;
}

class animation
{
private:
    int  current_frame      = 0;
    int  frame_inc          = 1;
    std::chrono::high_resolution_clock::time_point old_time;
    bool frame_locked = false;

    static inline auto now() { return std::chrono::high_resolution_clock::now(); }

public:
    std::chrono::high_resolution_clock::duration frame_rate  = 100ms;
    int  max_frame    = 1;

    enum rotate_type
    {
        none,
        circle,
        oscillate
    };
    rotate_type rotate_type_ = none;

public:
    animation(int max_frame, rotate_type t): max_frame {max_frame}, rotate_type_ {t} {}

    void animate()
    {
        if (frame_locked or (old_time + frame_rate > now()))
            return;

        old_time = now();
        current_frame += frame_inc;

        if (rotate_type_ == rotate_type::oscillate)
        {
            if ((frame_inc > 0 && current_frame >= max_frame) ||
                (current_frame <= 0))
                frame_inc = -frame_inc;
        }
        else if (rotate_type_ == rotate_type::circle)
        {
            if (frame_inc > 0)
            {
                if (current_frame >= max_frame)
                    current_frame = 0;
            }
            else
            {
                if (current_frame < 0)
                    current_frame = max_frame - 1;
            }
        }
        else if (current_frame >= max_frame)
            current_frame = 0;
    }

public:
    animation& set_current_frame(int f) { if (f < 0 || f >= max_frame) return *this; current_frame = f; return *this;}
    animation& lock()   {frame_locked = true; return *this;}
    animation& unlock() {frame_locked = false; return *this;}
    int get_current_frame() { return current_frame; }
};

} // namespace game
#endif // ANIMATION_HPP_
