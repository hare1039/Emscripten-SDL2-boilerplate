#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

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
    bool oscillate     = false;
    
public:
    animation(int max_frame): max_frame{max_frame} {}  

    void animate()
    {
        if (old_time + frame_rate > SDL_GetTicks())
            return;

        old_time = SDL_GetTicks();
        current_frame += frame_inc;

        if (oscillate)
        {
            if ((frame_inc > 0 && current_frame >= max_frame) || 
                (current_frame <= 0))            
                frame_inc = -frame_inc;
        }
        else if (current_frame >= max_frame)
            current_frame = 0;
    }

public:
    void set_current_frame(int f) { if (f < 0 || f >= max_frame) return; current_frame = f;}
    int  get_current_frame() { return current_frame; }
};

#endif // __ANIMATION_HPP__
