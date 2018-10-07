#ifndef DECL_HPP_
#define DECL_HPP_
#pragma once

namespace game
{

using error_code = int;
using pixel = int;
using SDL_Texture_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using SDL_Surface_ptr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;

constexpr pixel WINDOW_WIDTH_PIXEL  = 600;
constexpr pixel WINDOW_HEIGHT_PIXEL = 600;
constexpr int   MAP_WIDTH           = 8;
constexpr int   MAP_HEIGHT          = 8;
constexpr pixel TILE_SIZE_PIXEL     = 80;

} // namespace game


#endif // DECL_HPP_
