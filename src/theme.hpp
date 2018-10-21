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

public:
    theme(SDL_Renderer * r, std::string path):
        renderer {r},
        theme_camera {std::make_unique<camera>()}
    {
        // std::shared_ptr<cpptoml::table>
        auto config = cpptoml::parse_file(path);

        theme_area  = std::make_unique<area>(renderer, *(config->get_as<std::string>("area")));
        theme_camera->mode_id = static_cast<camera::mode>(*(config->get_as<int>("camera_mode")));

        for (const auto &table : *(config->get_table_array("elements")))
        {
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            elements.emplace_back(std::make_unique<element>(renderer, name));
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
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            movables.emplace_back(std::make_unique<movable>(renderer, name));
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

    void calculate()
    {
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
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
        std::for_each (element::all_elements().begin(), element::all_elements().end(),
                       [] (auto &e) { e.second->render(); });

    }

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override
    {
        switch (key)
        {
        case SDLK_UP:
            element::all_elements()["yoshi"]->jump();
            break;

        case SDLK_DOWN:
            for (auto && p : element::all_elements())
                std::cout << p.first << " -> " << p.second << std::endl;
            break;

        case SDLK_LEFT:
            element::all_elements()["yoshi"]->move_left = true;
            break;

        case SDLK_RIGHT:
            element::all_elements()["yoshi"]->move_right = true;
            break;

        case SDLK_SPACE:
            element::all_elements()["yoshi"]->jump();
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
            element::all_elements()["yoshi"]->move_left = false;
            break;

        case SDLK_RIGHT:
            element::all_elements()["yoshi"]->move_right = false;
            break;

        default:
            break;
        }
    }
};

} // namespace game

#endif // THEME_HPP_
