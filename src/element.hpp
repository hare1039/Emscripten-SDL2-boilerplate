#ifndef __ELEMENT_HPP__
#define __ELEMENT_HPP__
#pragma once

#include <string>
#include <memory>
#include "basic_headers.hpp"

namespace game
{

using velocity = int;

class element
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Rect      dest;
    SDL_Texture  *texture  = nullptr;
    unsigned int  active_state = NOTHING_PRESSED;
    velocity      vx = 0;
    velocity      vy = 0;

    element(SDL_Renderer *r): renderer{r} {}
    
    error_code set_texture(std::string path)
    {
        auto free_surf = [] (SDL_Surface * s) { SDL_FreeSurface (s); };
        std::unique_ptr<SDL_Surface, decltype(free_surf)> image {IMG_Load(path.c_str()), free_surf};
        if (not image)
        {
            std::cout << IMG_GetError() << std::endl;
            return -1;
        }
        texture = SDL_CreateTextureFromSurface (renderer, image.get());
        dest.w = image->w;
        dest.h = image->h;

        return 0;
    }
};



}// namespace game
#endif // __ELEMENT_HPP__
