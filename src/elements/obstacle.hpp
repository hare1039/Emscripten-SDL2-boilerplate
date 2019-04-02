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
        state_.dest_.x  = table->get_as<pixel>("x").value_or(state_.dest_.x);
        state_.dest_.y  = table->get_as<pixel>("y").value_or(state_.dest_.y);
        flag_ = static_cast<element::flag>(table->get_as<int>("flag")
                                           .value_or(cast(flag_)));
        col_offset_ = table->get_as<pixel>("offset").value_or(0);

        bounce_x_ = static_cast<bounce_direction>(table->get_as<int>("bounce_x")
                                                  .value_or(cast(bounce_x_)));
        bounce_y_ = static_cast<bounce_direction>(table->get_as<int>("bounce_y")
                                                  .value_or(cast(bounce_y_)));

        state_.dest_.w  = table->get_as<pixel>("w").value_or(state_.dest_.w);
        state_.dest_.h  = table->get_as<pixel>("h").value_or(state_.dest_.h);
    }

    void render()  override {/* invisible element */}
    void animate() override {/* invisible element */}
};

}


#endif // ELEMENTS_OBSTACLE_HPP_
