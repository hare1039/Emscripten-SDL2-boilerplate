#ifndef DECL_HPP_
#define DECL_HPP_
#pragma once

namespace game
{

using error_code = int;
using pixel      = double;
using SDL_Texture_ptr = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
using SDL_Surface_ptr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
using SDL_Window_ptr  = std::unique_ptr<SDL_Window,  decltype(&SDL_DestroyWindow)>;
using SDL_Renderer_ptr= std::unique_ptr<SDL_Renderer,decltype(&SDL_DestroyRenderer)>;

constexpr double ZOOM_FACTOR = 2;
constexpr pixel  WINDOW_WIDTH_PIXEL      = 450 * ZOOM_FACTOR;
constexpr pixel  WINDOW_HEIGHT_PIXEL     = 330 * ZOOM_FACTOR;
constexpr pixel  TILE_SIZE_PIXEL         = 30  * ZOOM_FACTOR;
constexpr int    WINDOW_WIDTH_PIXEL_INT  = WINDOW_WIDTH_PIXEL;
constexpr int    WINDOW_HEIGHT_PIXEL_INT = WINDOW_HEIGHT_PIXEL;
constexpr int    TILE_SIZE_PIXEL_INT     = TILE_SIZE_PIXEL;

constexpr int    MAP_WIDTH               = 3;
constexpr int    MAP_HEIGHT              = 3;
constexpr pixel  MAP_WIDTH_PIXEL         = MAP_WIDTH  * TILE_SIZE_PIXEL;
constexpr pixel  MAP_HEIGHT_PIXEL        = MAP_HEIGHT * TILE_SIZE_PIXEL;
constexpr int    MAP_WIDTH_PIXEL_INT     = MAP_WIDTH_PIXEL;
constexpr int    MAP_HEIGHT_PIXEL_INT    = MAP_HEIGHT_PIXEL;

constexpr double FPS = 16.0;

enum class next_operation
{
    skip,
    cont
};

} // namespace game


#endif // DECL_HPP_
