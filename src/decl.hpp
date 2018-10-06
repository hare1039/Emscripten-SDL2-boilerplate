#ifndef __DECL_HPP__
#define __DECL_HPP__
#pragma once

namespace game
{

using error_code = int;
using pixel = int;
using SDL_Texture_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using SDL_Surface_ptr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

constexpr pixel WINDOW_WIDTH_PIXEL  = 900;
constexpr pixel WINDOW_HEIGHT_PIXEL = 600;
constexpr int   MAP_WIDTH           = 5;
constexpr int   MAP_HEIGHT          = 5;
constexpr pixel TILE_SIZE_PIXEL     = 16;

} // namespace game


#endif // __DECL_HPP__
