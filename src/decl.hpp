#ifndef __DECL_HPP__
#define __DECL_HPP__
#pragma once

namespace game
{
    
using error_code = int;
using SDL_Texture_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using SDL_Surface_ptr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
    
} // namespace game


#endif // __DECL_HPP__
