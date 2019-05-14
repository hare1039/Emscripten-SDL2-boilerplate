#ifndef MAP_HPP_
#define MAP_HPP_

#pragma once

#include <vector>
#include <fstream>
#include <SDL2/SDL_render.h>
#include "basic_headers.hpp"
#include "tile.hpp"

namespace game
{

class map
{
    std::vector<tile> tiles_;
public:
    SDL_Renderer     *renderer_     = nullptr;
    SDL_Surface      *tile_surface_ = nullptr;
    SDL_Texture_ptr   map_texture_ {nullptr, &SDL_DestroyTexture};

public:
    map (std::string_view path, SDL_Renderer * r, SDL_Surface * tile_s)
        : renderer_ {r}, tile_surface_ {tile_s}
    {
        std::ifstream level_file {path.data()};
        std::string token;
        while (level_file >> token)
        {
            // expect token look like %d:%d
            try
            {
                auto pos      = token.find(":");
                int flag_num  = std::stoi(token.substr(0, pos)),
                    graph_num = std::stoi(token.substr(pos + 1));
                tiles_.emplace_back (tile::cast<'g'>(graph_num), tile::cast<'f'>(flag_num));
            } catch (std::invalid_argument const & e) {
                std::cout << "map parse failed" << std::endl;
            }
        }
        gen_texture();
    }

    void render (pixel map_x_pixel, pixel map_y_pixel)
    {
        int w = 0, h = 0;
        if (error_code ec = SDL_QueryTexture(
                map_texture_.get(),
                NULL, // format
                NULL, // access
                &w, &h);
            ec < 0)
            return;
        SDL_Rect dest {
            .x = static_cast<int>(map_x_pixel),
            .y = static_cast<int>(map_y_pixel),
            .w = static_cast<int>(w),
            .h = static_cast<int>(h)
        };
        SDL_RenderCopy (renderer_, map_texture_.get(), NULL /* full texture */, &dest);
    }

    inline tile& at(int x, int y) { return tiles_.at(utility::get_array_pos(x, y, MAP_WIDTH)); }

private:
    void gen_texture()
    {
        Uint32 rmask = 0, gmask = 0, bmask = 0, amask = 0;
        if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        }
        else
        {
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
        }

        SDL_Surface_ptr s {SDL_CreateRGBSurface(
            0,                         // no flags
            MAP_WIDTH  * TILE_SIZE_PIXEL,
            MAP_HEIGHT * TILE_SIZE_PIXEL,
            32,                        // color depth
            rmask, gmask, bmask, amask // color RGBA
        ), &SDL_FreeSurface};

        for (int x = 0; x < MAP_HEIGHT; x++)
        {
            for (int y = 0; y < MAP_WIDTH; y++)
            {
                tile &t {tiles_.at (utility::get_array_pos(x, y, MAP_WIDTH))};
                if (t.graph_ == tile::graph::none)
                    continue;
                SDL_Rect dest = {
                    .x = x * TILE_SIZE_PIXEL_INT,
                    .y = y * TILE_SIZE_PIXEL_INT,
                    .w = TILE_SIZE_PIXEL_INT,
                    .h = TILE_SIZE_PIXEL_INT
                };
                SDL_BlitSurface(tile_surface_, &t.src_, s.get(), &dest);
            }
        }

        map_texture_.reset (SDL_CreateTextureFromSurface (renderer_, s.get()));
    }
};

} // namespace game

#endif // MAP_HPP_
