#ifndef ELEMENTS_WOBBLE_HPP_
#define ELEMENTS_WOBBLE_HPP_

#pragma once

#include <string_view>
#include "../basic_headers.hpp"
#include "../fps.hpp"
#include "floating.hpp"

namespace game::elements::types
{

class wobble : public floating
{
    double duration_ = 1000; // ms
    pixel min_amplsize_pixel_ = 1;
    pixel max_amplsize_pixel_ = 1;
    enum class zoom {amplify = 1, condense = -1};
    zoom dir_ = zoom::amplify; // flag to move element big / small
public:
    wobble(SDL_Renderer *r,
           std::string_view name,
           cache_container<std::string, element> &a,
           camera &c,
           std::unique_ptr<fps>* game_fps): floating {r, name, a, c, game_fps} {}

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        floating::build_from_toml(table);
        duration_           = table->get_as<double>("duration").value_or(duration_);
        max_amplsize_pixel_ = table->get_as<double>("max_ampl").value_or(max_amplsize_pixel_) * state_.dest_.w;
        min_amplsize_pixel_ = table->get_as<double>("min_ampl").value_or(min_amplsize_pixel_) * state_.dest_.w;
        state_.dest_.w     *= table->get_as<double>("start_ampl").value_or(1);
        if (state_.dest_.w <= 0)
            state_.dest_.w = 1;
    }

    void calculate() override
    {
        floating::calculate();
        pixel step = (max_amplsize_pixel_ - min_amplsize_pixel_) / (duration_ / (*game_fps_)->frame());
        if (state_.dest_.w < min_amplsize_pixel_)
            dir_ = zoom::amplify;
        else if (state_.dest_.w > max_amplsize_pixel_)
            dir_ = zoom::condense;
        amplify_to(state_.dest_.w + utility::cast(dir_) * step);
    }
};

}// namespace game::elements::types

#endif // ELEMENTS_WOBBLE_HPP_
