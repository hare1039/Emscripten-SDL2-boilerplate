#ifndef THEME_HPP_
#define THEME_HPP_

#include <vector>
#include <unordered_map>
#include "basic_headers.hpp"
#include "event.hpp"
#include "elements/floating.hpp"
#include "elements/movable.hpp"
#include "camera.hpp"
#include "area.hpp"
#include "fps.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class app;

class theme : public event
{
protected:
    friend app;
    SDL_Renderer *          renderer    {nullptr};
    std::unique_ptr<area>   theme_area  {nullptr};
    std::unique_ptr<camera> theme_camera{std::make_unique<camera>()};
    std::unique_ptr<theme>  next_theme  {nullptr};
    std::unordered_map<std::string, std::unique_ptr<element>> elements;

public:
    theme(SDL_Renderer * r, std::string_view path):
        renderer {r}
    {
        // std::shared_ptr<cpptoml::table>
        auto config = cpptoml::parse_file(path.data());

        theme_area  = std::make_unique<area>(renderer, *(config->get_as<std::string>("area")));
        auto cam    = config->get_table("camera");
        theme_camera->mode_id = static_cast<camera::mode>(cam->get_as<int>("mode")
                                                          .value_or(cast(camera::mode::top_left)));
        theme_camera->set(cam->get_as<int>("x").value_or(0),
                          cam->get_as<int>("y").value_or(0));

        build<element>("elements", config);
        build<element_types::movable> ("movables", config);
        build<element_types::floating>("floatings", config);
    }

    virtual
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

    virtual
    void render()
    {
        auto [x, y] = theme_camera->get_pos();
        theme_area->render (x, y);
        std::for_each (elements.begin(), elements.end(),
                       [] (auto &e) { e.second->render(); });
    }

    virtual
    std::unique_ptr<theme> next() { return std::move(next_theme); }
    virtual
    bool is_finished() { return (!! next_theme); }

private:
    template<typename T, typename ... Args>
    void build(std::string toml_name, std::shared_ptr<cpptoml::table> config, Args && ... args)
    {
        for (const auto &table : *(config->get_table_array(toml_name)))
        {
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            elements.emplace(name, std::make_unique<T>(renderer, name, elements, *theme_camera,
                                                       std::forward<Args>(args)...));
            elements[name]->build_from_toml(table);
            if (table->get_as<bool>("bind_cam"))
                theme_camera->bind(&elements[name]->dest);
        }
    }
};

} // namespace game

#endif // THEME_HPP_
