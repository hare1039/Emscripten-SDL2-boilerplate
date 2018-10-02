#ifndef __DECL_HPP__
#define __DECL_HPP__
#pragma once

namespace game
{

enum input_state
{
    NOTHING_PRESSED = 0,
    UP_PRESSED      = 1,
    DOWN_PRESSED    = 1 << 1,
    LEFT_PRESSED    = 1 << 2,
    RIGHT_PRESSED   = 1 << 3
};

using error_code = int;

constexpr double SPEED_DOWN_RATE = 1.3; //1.4142135624;
constexpr double INV = 1.0 / SPEED_DOWN_RATE;


    
} // namespace game


#endif // __DECL_HPP__
