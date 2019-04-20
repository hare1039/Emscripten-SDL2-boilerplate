#ifndef ELEMENTS_OBSTACLE_HPP_
#define ELEMENTS_OBSTACLE_HPP_

#include "../basic_headers.hpp"
#include "../element.hpp"

namespace game::element_types
{

class obstacle : public element
{
public:
    obstacle(SDL_Renderer *r,
             std::string_view name,
             cache_container<std::string, element> &a,
             camera &c): element{r, name, a, c}
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

}


#endif // ELEMENTS_OBSTACLE_HPP_
