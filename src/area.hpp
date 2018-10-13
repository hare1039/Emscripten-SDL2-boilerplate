#ifndef AREA_HPP_
#define AREA_HPP_

#pragma once

#include <deque>
#include <algorithm>
#include <fstream>
#include <memory>
#include "basic_headers.hpp"
#include "enable_instance.hpp"
#include "map.hpp"

namespace game
{

class area : public enable_instance<area>
{
public:
    std::vector<map> maps;
    int area_width  = 0;
    int area_height = 0;
    SDL_Surface_ptr tile_surface {nullptr, &SDL_FreeSurface};
    SDL_Renderer  * renderer = nullptr;

public:
    area (SDL_Renderer * r, std::string path): renderer{r}
    {
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
        std::cout << path << " builded" << std::endl;
    }

    void render (pixel camera_x_pixel, pixel camera_y_pixel)
    {
        constexpr pixel MAP_WIDTH_PIXEL  = MAP_WIDTH  * TILE_SIZE_PIXEL;
        constexpr pixel MAP_HEIGHT_PIXEL = MAP_HEIGHT * TILE_SIZE_PIXEL;

        int map_id_x = utility::get_array_pos(camera_x_pixel / MAP_WIDTH_PIXEL,
                                              camera_y_pixel / MAP_HEIGHT_PIXEL,
                                              area_width);
        int map_id_x_end = utility::get_array_pos((camera_x_pixel + WINDOW_WIDTH_PIXEL) / MAP_WIDTH_PIXEL,
                                                  camera_y_pixel / MAP_HEIGHT_PIXEL,
                                                  area_width);
        int map_id_y_shift = utility::get_array_pos(camera_x_pixel / MAP_WIDTH_PIXEL,
                                                    (camera_y_pixel + WINDOW_HEIGHT_PIXEL) / MAP_HEIGHT_PIXEL,
                                                    area_width) - map_id_x;

        for (int i = map_id_x; i <= map_id_x_end; i++)
        {
            for (int id = i; id <= i + map_id_y_shift; id += area_width)
            {
                if (id < 0 or id >= static_cast<int>(maps.size()))
                    continue;
                pixel x = ((id % area_width) * MAP_WIDTH_PIXEL)  - camera_x_pixel;
                pixel y = ((id / area_width) * MAP_HEIGHT_PIXEL) - camera_y_pixel;
                maps[id].render(x, y);
            }
        }
    }

    tile& at_map(int x, int y)
    {
        int area_x = x / MAP_WIDTH;
        int area_y = y / MAP_HEIGHT;

        int mapid = utility::get_array_pos(area_x, area_y, area_width);
        return maps.at(mapid).at(x % MAP_WIDTH, y % MAP_HEIGHT);
    }
};

}
#endif // AREA_HPP_
