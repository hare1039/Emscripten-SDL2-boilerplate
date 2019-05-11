#ifndef ELEMENTS_OBSTACLE_HPP_
#define ELEMENTS_OBSTACLE_HPP_

#include "../basic_headers.hpp"
#include "../element.hpp"

namespace game::elements::types
{

class obstacle : public element
{
public:
    obstacle(std::string_view name,
             cache_container<std::string, element> &a,
             SDL_Renderer *r,
             camera &c,
             std::unique_ptr<fps>* game_fps): element {name, a, r, c, game_fps}
    {
        col_offset_ = 0;
    }

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        build_from_toml_basic(table);

        state_.dest_.w  = table->get_as<pixel>("w").value_or(state_.dest_.w);
        state_.dest_.h  = table->get_as<pixel>("h").value_or(state_.dest_.h);
    }

    void render()  override {/* invisible element */}
    void animate() override {/* invisible element */}
};

} // namespace game::elements::types

#endif // ELEMENTS_OBSTACLE_HPP_
