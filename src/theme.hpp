#ifndef THEME_HPP_
#define THEME_HPP_

#include <vector>
#include <unordered_map>
#include <string_view>
#include "elements/register.hpp"
#include "basic_headers.hpp"
#include "event.hpp"
#include "camera.hpp"
#include "area.hpp"
#include "cache_container.hpp"
#include "theme_animation.hpp"
#include "external_libraries/cpptoml/include/cpptoml.h"

namespace game
{

class app;

class theme : public event
{
protected:
    friend app;
    SDL_Renderer *          renderer    {nullptr};
    std::unique_ptr<fps> *  game_fps    {nullptr};
    std::unique_ptr<area>   theme_area  {nullptr};
    std::unique_ptr<camera> theme_camera{std::make_unique<camera>()};
    std::unique_ptr<theme>  next_theme  {nullptr};
    theme_animation         animation;
    cache_container<std::string, element> elements {[](element *lhs, element *rhs){ return lhs->z_index_ < rhs->z_index_; }};

public:
    theme(SDL_Renderer * r, std::unique_ptr<fps>* gfps, std::string_view path):
        renderer {r},
        game_fps {gfps}
    {
        auto config = cpptoml::parse_file(path.data());
        elements::types::builder(elements, config, renderer, std::ref(*theme_camera), game_fps);

        assert(config->contains("area") and
               config->contains("camera"));

        theme_area  = std::make_unique<area>(renderer, *(config->get_as<std::string>("area")));
        auto cam    = config->get_table("camera");
        theme_camera->mode_id = static_cast<camera::mode>(cam->get_as<int>("mode")
                                                          .value_or(utility::cast(camera::mode::top_left)));
        theme_camera->set(cam->get_as<int>("x").value_or(0),
                          cam->get_as<int>("y").value_or(0));
        std::string bind_element = cam->get_as<std::string>("bind").value_or("");

        if (not bind_element.empty())
            theme_camera->bind(&elements[bind_element]->state_.dest_);
    }

    virtual
    void calculate()
    {
        if (animation.is_running())
            animation.calculate();

        std::for_each (elements.begin(), elements.end(),
                       [] (auto &e) { e.second->calculate(); });
        std::for_each (element::collision::queue().begin(), element::collision::queue().end(),
                       [] (element::collision & col) {
                           col.A.on_collision(col.B);
                           col.B.on_collision(col.A);
                       });
        element::collision::queue().clear();
    }

    virtual
    void render()
    {
        auto [x, y] = theme_camera->get_pos();
        theme_area->render (x, y);
        std::for_each (elements.cache_begin(), elements.cache_end(),
                       [] (element *e) { e->render(); });
    }

    virtual std::unique_ptr<theme> next() { return std::move(next_theme); }
    virtual bool is_finished() { return (!! next_theme); }

protected: // animation related functions
    template<typename ... Rest> inline
    void bind_animation_fps(element &e, Rest && ... rest)
    {
        e.bind_fps(animation.get_fps());
        bind_animation_fps(std::forward<Rest>(rest)...);
    }

    template<typename ... Element> inline
    void enable_animation (Element && ... e)
    {
        (*game_fps)->pause();
        bind_animation_fps(std::forward<Element>(e)...);
    }

    std::function<void(void)> default_resume () { return [this]{ default_resume_impl(); }; };

private:
    // perform default game resume process
    // fps resume and all element binds to game_fps
    void default_resume_impl()
    {
        (*game_fps)->resume();
        std::for_each(elements.begin(), elements.end(),
                      [this] (auto &e) { e.second->bind_fps(game_fps); });
    }


    inline void bind_animation_fps() {}
};

} // namespace game

#endif // THEME_HPP_
