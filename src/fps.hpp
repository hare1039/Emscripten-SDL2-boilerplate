#ifndef FPS_HPP_
#define FPS_HPP_

#pragma once
#include "basic_headers.hpp"
#include "enable_instance.hpp"

namespace game
{

using ulli = unsigned long long int;
ulli operator "" _s (ulli v) { return v * 1000; }

class fps : public enable_instance<fps>
{
    double _speed_factor = 0;
    int old_time  = 0;
    int last_time = 0;
    int frames = 0;
    int number_frames = 0;
public:
    void calculate()
    {
        if (old_time + 1_s < SDL_GetTicks())
        {
            old_time = SDL_GetTicks();
            number_frames = frames;
            frames = 0;
        }

        _speed_factor = ((SDL_GetTicks() - last_time) / 1000.0) * FPS;
        last_time = SDL_GetTicks();
        frames++;
    }

    int frame() { return number_frames; }
    double speed_factor() { return _speed_factor; }
};

} // namespace game

#endif // FPS_HPP_
