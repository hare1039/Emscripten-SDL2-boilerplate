#ifndef THEME_HPP_
#define THEME_HPP_

#include <vector>
#include "basic_headers.hpp"
#include "event.hpp"
#include "movable.hpp"
#include "camera.hpp"
#include "area.hpp"
#include "fps.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class app;

class theme : public event
{
    friend app;
    SDL_Renderer *          renderer = nullptr;
    std::vector<std::unique_ptr<element>> elements;
    std::vector<std::unique_ptr<movable>> movables;
    std::unique_ptr<area>   theme_area;
    std::unique_ptr<camera> theme_camera;
    std::unique_ptr<fps>    theme_fps;

public:
    theme(SDL_Renderer * r, std::string path):
        renderer {r},
        theme_camera {std::make_unique<camera>()},
        theme_fps {std::make_unique<fps>()}
    {
        // std::shared_ptr<cpptoml::table>
        auto config = cpptoml::parse_file(path);

        theme_area  = std::make_unique<area>(renderer, *(config->get_as<std::string>("area")));
        theme_camera->mode_id = static_cast<camera::mode>(*(config->get_as<int>("camera_mode")));

        for (const auto &table : *(config->get_table_array("elements")))
        {
            elements.emplace_back(std::make_unique<element>(renderer));
            if (error_code ec = elements.back()->set_texture(
                *(table->get_as<std::string>("pic")),
                *(table->get_as<int>("width")),
                *(table->get_as<int>("height")),
                static_cast<animation::rotate_type>(*(table->get_as<int>("rotate_t")))); ec < 0)
                std::cout << SDL_GetError() << std::endl;;

            elements.back()->dest.x = *(table->get_as<pixel>("x"));
            elements.back()->dest.y = *(table->get_as<pixel>("y"));
            auto bind_cam = table->get_as<bool>("bind_cam");
            if (bind_cam)
                theme_camera->bind(&elements.back()->dest);
        }

        for (const auto &table : *(config->get_table_array("movables")))
        {
            movables.emplace_back(std::make_unique<movable>(renderer));
            if (error_code ec = movables.back()->set_texture(
                *(table->get_as<std::string>("pic")),
                *(table->get_as<int>("width")),
                *(table->get_as<int>("height")),
                static_cast<animation::rotate_type>(*(table->get_as<int>("rotate_t")))); ec < 0)
                std::cout << "Load movables image error" << std::endl;;

            movables.back()->dest.x  = *(table->get_as<pixel>("x"));
            movables.back()->dest.y  = *(table->get_as<pixel>("y"));
            movables.back()->flag_id = static_cast<element::flag>(*(table->get_as<int>("flag_id")));

            auto bind_cam = table->get_as<bool>("bind_cam");
            if (bind_cam)
                theme_camera->bind(&movables.back()->dest);
        }
    }

    void on_key_down(SDL_Keycode const &, Uint16 const &) override {}
    void on_key_up  (SDL_Keycode const &, Uint16 const &) override {}
};

} // namespace game

#endif // THEME_HPP_
