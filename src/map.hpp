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
    std::vector<tile> tiles;
public:    
    SDL_Renderer     *renderer     = nullptr;
    SDL_Surface      *tile_surface = nullptr;
    SDL_Texture_ptr   map_texture{nullptr, &SDL_DestroyTexture};
    
public:
    map (std::string path, SDL_Renderer * r, SDL_Surface * tile_s)
        : renderer{r}, tile_surface{tile_s}
    {
        std::ifstream level_file {path};
        std::string token;
        while (level_file >> token)
        {
            // expect token look like %d:%d
            try
            {
                auto pos      = token.find(":");
                int  id_num   = std::stoi(token.substr(0, pos)),
                     type_num = std::stoi(token.substr(pos + 1));
                tiles.emplace_back (id_num, tile::cast(type_num));
            } catch (std::invalid_argument const & e) {
                std::cout << "map parse failed" << std::endl;
            }
        }
        std::cout << path << " loaded" << std::endl;
        gen_texture();
    }
    
    void render (pixel map_x_pixel, pixel map_y_pixel)
    {
        int w = 0, h = 0;
        if (error_code ec = SDL_QueryTexture(
                map_texture.get(),
                NULL, // format
                NULL, // access
                &w, &h);
            ec < 0)
            return;
        SDL_Rect dest = {
            .x = map_x_pixel,
            .y = map_y_pixel,
            .h = h,
            .w = w
        };
        SDL_RenderCopy (renderer, map_texture.get(), NULL /* full texture */, &dest);
    }

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
                tile &t {tiles.at (utility::get_array_pos(x, y, MAP_WIDTH))};
                if (t.type_id == tile::type::none)
                    continue;
                SDL_Rect dest = {
                    .x = x * TILE_SIZE_PIXEL,
                    .y = y * TILE_SIZE_PIXEL,
                    .h = TILE_SIZE_PIXEL,
                    .w = TILE_SIZE_PIXEL
                };
                SDL_BlitSurface(tile_surface, &t.src, s.get(), &dest);
            }
        }

        map_texture.reset (SDL_CreateTextureFromSurface (renderer, s.get()));
    }
};

} // namespace game

#endif // MAP_HPP_
