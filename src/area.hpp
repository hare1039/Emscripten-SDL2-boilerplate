#ifndef __AREA_HPP__
#define __AREA_HPP__

#pragma once

#include <deque>
#include <algorithm>
#include <fstream>
#include "basic_headers.hpp"
#include "map.hpp"

namespace game
{

class area
{
public:
    static
    std::deque<area *> & all_areas()
    {
        static std::deque<area *> _v;
        return _v;
    }

    std::vector<map> maps;
    int area_width  = 0;
    int area_height = 0; 
    SDL_Surface_ptr tile_surface {nullptr, &SDL_FreeSurface};
    SDL_Renderer  * renderer = nullptr;

public:
    area (SDL_Renderer * r, std::string path): renderer{r}
    {
        all_areas().push_back(this);
        std::ifstream area_file{path};
        std::string tile_path;
        area_file >> tile_path;
        tile_surface.reset(IMG_Load(tile_path.c_str()));
        if (not tile_surface)
            std::cout << IMG_GetError() << std::endl;

        area_file >> area_width >> area_height;

        for (int i = 0; i < area_width; i++)
            for (int j = 0; j < area_height; j++)
            {
                std::string map_path;
                area_file >> map_path;
                maps.emplace_back(map_path, renderer, tile_surface.get());
            }
    }
    
    ~area ()
    {
        std::deque<area *> all = all_areas();
        auto it = std::remove(all.begin(), all.end(), this);
        all.erase(it, all.end());
    }

    void render (pixel camera_x_pixel, pixel camera_y_pixel)
    {
        constexpr pixel MAP_WIDTH_PIXEL  = MAP_WIDTH  * TILE_SIZE_PIXEL;
        constexpr pixel MAP_HEIGHT_PIXEL = MAP_HEIGHT * TILE_SIZE_PIXEL;

        int left_up = -camera_x_pixel / MAP_WIDTH_PIXEL;
        left_up += ((-camera_y_pixel / MAP_HEIGHT_PIXEL) * area_width);

        for (int i = 0; i < 4; i++)
        {
            unsigned long id = left_up + ((i / 2) * area_width) + (i % 2);

            if (id < 0 or id >= maps.size())
                continue;

            pixel x = ((id % area_width) * MAP_WIDTH_PIXEL)  + camera_x_pixel;
            pixel y = ((id / area_width) * MAP_HEIGHT_PIXEL) + camera_y_pixel;

            maps[id].render(x, y);
        }
    }
};

}
#endif // __AREA_HPP__
