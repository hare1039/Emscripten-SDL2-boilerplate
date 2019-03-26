#ifndef ELEMENTS_WOBBLE_HPP_
#define ELEMENTS_WOBBLE_HPP_

#pragma once

#include <string_view>
#include "../basic_headers.hpp"
#include "../fps.hpp"
#include "floating.hpp"

namespace game::element_types
{

class wobble : public floating
{
    double duration = 1000; // ms
    pixel min_amplsize_pixel = 1;
    pixel max_amplsize_pixel = 1;
    enum class zoom {amplify = 1, condense = -1};
    zoom dir_id = zoom::amplify; // flag to move element big / small
public:
    wobble(SDL_Renderer *r,
           std::string_view name,
           std::unordered_map<std::string, std::unique_ptr<element>> &a,
           camera &c): floating{r, name, a, c} {}

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        floating::build_from_toml(table);
        duration           = table->get_as<double>("duration").value_or(duration);
        max_amplsize_pixel = table->get_as<double>("max_ampl").value_or(max_amplsize_pixel) * dest.w;
        min_amplsize_pixel = table->get_as<double>("min_ampl").value_or(min_amplsize_pixel) * dest.w;
        dest.w            *= table->get_as<double>("start_ampl").value_or(1);
        if (dest.w <= 0)
            dest.w = 1;
    }

    void calculate() override
    {
        floating::calculate();
        pixel step = (max_amplsize_pixel - min_amplsize_pixel) / (duration / fps::instance()->frame());
        if (dest.w < min_amplsize_pixel)
            dir_id = zoom::amplify;
        else if (dest.w > max_amplsize_pixel)
            dir_id = zoom::condense;
        amplify_to(dest.w + utility::cast(dir_id) * step);
    }
};

}// namespace game::element

#endif // ELEMENTS_WOBBLE_HPP_
