#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#pragma once

#include "basic_headers.hpp"

class animation
{
private:
    int  current_frame      = 0;
    int  frame_inc          = 1;
    unsigned long old_time  = 0;

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
        if (old_time + frame_rate > SDL_GetTicks())
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
    void set_current_frame(int f) { if (f < 0 || f >= max_frame) return; current_frame = f;}
    int  get_current_frame() { return current_frame; }
};

#endif // ANIMATION_HPP_
