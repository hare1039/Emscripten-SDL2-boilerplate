#ifndef BASIC_HEADERS_HPP_
#define BASIC_HEADERS_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#if __has_include(<emscripten.h>)
#   include <emscripten.h>
#endif // __has_include(<emscripten.h>)
#include "decl.hpp"
#include "utility.hpp"
#include "rect.hpp"
#include "point.hpp"
#include "compile_time_utility.hpp"
#include "enum_operator.hpp"

#endif // BASIC_HEADERS_HPP_
