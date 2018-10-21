#ifndef THEME_HPP_
#define THEME_HPP_

#include <vector>
#include <unordered_map>
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
    std::unique_ptr<area>   theme_area;
    std::unique_ptr<camera> theme_camera;
    std::string             next_theme;
    std::unordered_map<std::string, std::unique_ptr<element>> elements;

public:
    theme(SDL_Renderer * r, std::string path):
        renderer {r},
        theme_camera {std::make_unique<camera>()}
    {
        // std::shared_ptr<cpptoml::table>
        auto config = cpptoml::parse_file(path);

        theme_area  = std::make_unique<area>(renderer, *(config->get_as<std::string>("area")));
        auto cam    = config->get_table("camera");
        theme_camera->mode_id = static_cast<camera::mode>(*(cam->get_as<int>("mode")));
        theme_camera->shift(cam->get_as<int>("x").value_or(0), cam->get_as<int>("y").value_or(0));

        for (const auto &table : *(config->get_table_array("elements")))
        {
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            elements.emplace(name, std::make_unique<element>(renderer, name, elements));
            auto && new_element = elements[name];
            if (error_code ec = new_element->set_texture(
                *(table->get_as<std::string>("pic")),
                *(table->get_as<int>("width")),
                *(table->get_as<int>("height")),
                static_cast<animation::rotate_type>(*(table->get_as<int>("rotate_t")))); ec < 0)
                std::cout << SDL_GetError() << std::endl;;

            new_element->dest.x = *(table->get_as<pixel>("x"));
            new_element->dest.y = *(table->get_as<pixel>("y"));
            auto bind_cam = table->get_as<bool>("bind_cam");
            if (bind_cam)
                theme_camera->bind(&new_element->dest);
        }

        for (const auto &table : *(config->get_table_array("movables")))
        {
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            elements.emplace(name, std::make_unique<movable>(renderer, name, elements));
            auto && new_element = elements[name];
            if (error_code ec = new_element->set_texture(
                *(table->get_as<std::string>("pic")),
                *(table->get_as<int>("width")),
                *(table->get_as<int>("height")),
                static_cast<animation::rotate_type>(*(table->get_as<int>("rotate_t")))); ec < 0)
                std::cout << "Load movables image error" << std::endl;;

            new_element->dest.x  = *(table->get_as<pixel>("x"));
            new_element->dest.y  = *(table->get_as<pixel>("y"));
            new_element->flag_id = static_cast<element::flag>(*(table->get_as<int>("flag_id")));

            auto bind_cam = table->get_as<bool>("bind_cam");
            if (bind_cam)
                theme_camera->bind(&new_element->dest);
        }
    }

    void calculate()
    {
        std::for_each (elements.begin(), elements.end(),
                       [] (auto &e) { e.second->calculate(); });
        std::for_each (element::collision::queue().begin(), element::collision::queue().end(),
                       [] (element::collision & col) {
                           if (col.A.on_collision(col.B) == next_operation::cont)
                               col.B.on_collision(col.A);
                       });
        element::collision::queue().clear();
    }

    void render()
    {
        auto [x, y] = theme_camera->get_pos();
        theme_area->render (x, y);
        std::for_each (elements.begin(), elements.end(),
                       [] (auto &e) { e.second->render(); });
    }

    std::string next() { return next_theme; }
    bool is_finished() { return next_theme != ""; }

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override
    {
        switch (key)
        {
        case SDLK_UP:
            elements["yoshi"]->jump();
            break;

        case SDLK_DOWN:
            for (auto && p : elements)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme = "./asset/theme/01.toml";
            break;

        case SDLK_LEFT:
            elements["yoshi"]->move_left = true;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = true;
            break;

        case SDLK_SPACE:
            elements["yoshi"]->jump();
            elements["rin"]->dest.x += 100;
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        switch (key)
        {
        case SDLK_LEFT:
            elements["yoshi"]->move_left = false;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = false;
            break;

        default:
            break;
        }
    }
};

} // namespace game

#endif // THEME_HPP_
