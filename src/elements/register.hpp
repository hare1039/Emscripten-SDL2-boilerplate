#ifndef ELEMENTS_REGISTER_HPP_
#define ELEMENTS_REGISTER_HPP_

#include "../external_libraries/cpptoml/include/cpptoml.h"
#include "floating.hpp"
#include "movable.hpp"
#include "wobble.hpp"
#include "player.hpp"
#include "obstacle.hpp"
#include "ball.hpp"
#include "text.hpp"
#include "score_counter.hpp"
#include "option.hpp"
#include "fence.hpp"

namespace game::elements::types
{

namespace {

template<typename Element, typename BuildType, typename ... Args>
void build(BuildType&& elements, std::string_view toml_name, std::shared_ptr<cpptoml::table> config, Args && ... args)
{
    auto table_array = config->get_table_array(toml_name.data());
    if (table_array != nullptr)
        for (const auto &table : *table_array)
        {
            std::string name = table->
                get_as<std::string>("name").
                value_or(utility::random_string(20));

            elements.emplace(name, std::make_unique<Element>(name, elements, std::forward<Args>(args)...));
            elements[name]->build_from_toml(table);
        }
}

} // namespace

template <typename BuildType, typename ... Args>
void builder(BuildType&& elements, std::shared_ptr<cpptoml::table> config, Args ... args)
{
    build<element> (elements, "elements",  config, std::forward<Args>(args)...);
    build<movable> (elements, "movables",  config, std::forward<Args>(args)...);
    build<floating>(elements, "floatings", config, std::forward<Args>(args)...);
    build<wobble>  (elements, "wobbles",   config, std::forward<Args>(args)...);
    build<player>  (elements, "players",   config, std::forward<Args>(args)...);
    build<obstacle>(elements, "obstacles", config, std::forward<Args>(args)...);
    build<ball>    (elements, "balls",     config, std::forward<Args>(args)...);
    build<text>    (elements, "texts",     config, std::forward<Args>(args)...);
    build<score_counter> (elements, "score_counters", config, std::forward<Args>(args)...);
    build<option>  (elements, "options",   config, std::forward<Args>(args)...);
    build<fence>   (elements, "fences",    config, std::forward<Args>(args)...);
}

} // namespace game::self::types

#endif // ELEMENTS_REGISTER_HPP_
