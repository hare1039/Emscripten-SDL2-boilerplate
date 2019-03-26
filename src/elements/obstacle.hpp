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
             std::unordered_map<std::string, std::unique_ptr<element>> &a,
             camera &c): element{r, name, a, c} {}

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        dest.x  = table->get_as<pixel>("x").value_or(dest.x);
        dest.y  = table->get_as<pixel>("y").value_or(dest.y);
        flag_id = static_cast<element::flag>(table->get_as<int>("flag_id")
                                             .value_or(cast(flag_id)));
        col_offset = table->get_as<pixel>("offset").value_or(0);

        bounce_x = static_cast<bounce_direction>(table->get_as<int>("bounce_x")
                                                 .value_or(cast(bounce_x)));
        bounce_y = static_cast<bounce_direction>(table->get_as<int>("bounce_y")
                                                 .value_or(cast(bounce_y)));

        dest.w  = table->get_as<pixel>("w").value_or(dest.w);
        dest.h  = table->get_as<pixel>("h").value_or(dest.h);
    }

    void render()  override {/* invisible element */}
    void animate() override {/* invisible element */}
};

}


#endif // ELEMENTS_OBSTACLE_HPP_
