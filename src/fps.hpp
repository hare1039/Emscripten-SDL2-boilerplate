#ifndef FPS_HPP_
#define FPS_HPP_

#pragma once
#include "basic_headers.hpp"
#include "enable_instance.hpp"

namespace game
{

using ulli = unsigned long long int;
ulli operator "" _s (ulli v) { return v * 1000; }
/*
** http://www.sdltutorials.com/sdl-collision **
 * Next, we have the actual calculation for the SpeedFactor.
 * Like I mentioned earlier, the SpeedFactor determines how fast to move objects.
 * Now, for a better explanation.
 * Say we are on a really slow computer that gets 1 FPS per second.
 * That's a really bad computer by the way.
 * We have the base movement rate set at 32 (our made up number).
 * So, ideally, any computer will move objects 32 pixels per second.
 * So this 1 FPS computer will move the object 32 pixels every second.
 * Now, jump to a 10 FPS computer.
 * We still want to move 32 pixels per second.
 * So, that's 1/10 of 32 right? So, for each calculation we move 3.2 pixels,
 * and after 1 second, we'll have moved 32 pixels.
 *
 * To calculate this, we base it on how fast the computer is currently going.
 * This is what LastTime is for.
 * LastTime holds the time it took for the last loop in the game.
 * So, in the constructor it's currently 0.
 * Now, say we have an FPS of 10.
 * That means we have 10 loops in the game every second.
 * How many milliseconds is that? That's 100 milliseconds.
 * So, 100 / 1000 (for 1 second), is 1/10.
 * The same number we got above.
 * Now multiply this by our desired movement rate, and we have our Speed Factor! I'll be showing you how to use this later on, just try to understand how we calculated it.
 *
 */

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
