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
    int  current_frame_ = 0;
    int  frame_inc_     = 1;
    std::chrono::high_resolution_clock::time_point old_time_;
    bool frame_locked_  = false;

    static inline auto now() { return std::chrono::high_resolution_clock::now(); }

public:
    std::chrono::high_resolution_clock::duration frame_rate_ = 100ms;
    int  max_frame_ = 1;

    enum rotate_type
    {
        none,
        circle,
        oscillate
    };
    rotate_type rotate_type_ = none;

public:
    animation(int max_frame, rotate_type t): max_frame_ {max_frame}, rotate_type_ {t} {}

    void animate()
    {
        if (frame_locked_ or (old_time_ + frame_rate_ > now()))
            return;

        old_time_ = now();
        current_frame_ += frame_inc_;

        if (rotate_type_ == rotate_type::oscillate)
        {
            if ((frame_inc_ > 0 && current_frame_ >= max_frame_) ||
                (current_frame_ <= 0))
                frame_inc_ = -frame_inc_;
        }
        else if (rotate_type_ == rotate_type::circle)
        {
            if (frame_inc_ > 0)
            {
                if (current_frame_ >= max_frame_)
                    current_frame_ = 0;
            }
            else
            {
                if (current_frame_ < 0)
                    current_frame_ = max_frame_ - 1;
            }
        }
        else if (current_frame_ >= max_frame_)
            current_frame_ = 0;
    }

public:
    animation& set_current_frame(int f) { if (f < 0 || f >= max_frame_) return *this; current_frame_ = f; return *this;}
    animation& lock()   {frame_locked_ = true;  return *this;}
    animation& unlock() {frame_locked_ = false; return *this;}
    int get_current_frame() { return current_frame_; }
};

} // namespace game
#endif // ANIMATION_HPP_
