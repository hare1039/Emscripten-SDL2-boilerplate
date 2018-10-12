#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#pragma once

#include "basic_headers.hpp"

namespace game
{

class animation
{
private:
    int  current_frame      = 0;
    int  frame_inc          = 1;
    unsigned long old_time  = 0;
    bool frame_locked = false;

public:
    int  frame_rate    = 100; //Milliseconds
    int  max_frame     = 1;

    enum rotate_type
    {
        none,
        circle,
        oscillate
    } rotate_type_id = none;

public:
    animation(int max_frame, rotate_type t): max_frame{max_frame}, rotate_type_id{t} {}  

    void animate()
    {
        if (frame_locked or (old_time + frame_rate > SDL_GetTicks()))
            return;

        old_time = SDL_GetTicks();
        current_frame += frame_inc;

        if (rotate_type_id == rotate_type::oscillate)
        {
            if ((frame_inc > 0 && current_frame >= max_frame) || 
                (current_frame <= 0))            
                frame_inc = -frame_inc;
        }
        else if (rotate_type_id == rotate_type::circle)
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
