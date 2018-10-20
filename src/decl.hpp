#ifndef DECL_HPP_
#define DECL_HPP_
#pragma once

namespace game
{

using error_code = int;
using pixel = int;
using SDL_Texture_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using SDL_Surface_ptr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
using SDL_Window_ptr  = std::unique_ptr<SDL_Window,  decltype(&SDL_DestroyWindow)>;
using SDL_Renderer_ptr= std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)>;

constexpr pixel WINDOW_WIDTH_PIXEL  = 900;
constexpr pixel WINDOW_HEIGHT_PIXEL = 600;
constexpr int   MAP_WIDTH           = 8;
constexpr int   MAP_HEIGHT          = 8;
constexpr pixel TILE_SIZE_PIXEL     = 80;

constexpr double FPS = 32.0;

enum class next_operation
{
    skip,
    cont
};

} // namespace game


#endif // DECL_HPP_
