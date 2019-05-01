#ifndef THEME_ANIMATION_HPP_
#define THEME_ANIMATION_HPP_

#include "basic_headers.hpp"
#include "fps.hpp"
#include <memory>
#include <chrono>
#include <functional>

namespace game
{

namespace {
    using namespace std::literals;
}

class theme_animation
{
    std::chrono::high_resolution_clock::duration length_;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_ {std::chrono::high_resolution_clock::now() - 24h};
    std::function<void(void)> on_calculate_;
    std::function<void(void)> on_finish_ {[](){}};
    std::unique_ptr<fps>      fps_ {std::make_unique<fps>()};
public:

    theme_animation& set (std::chrono::high_resolution_clock::duration length,
                          std::function<void(void)> && on_calculate,
                          std::function<void(void)> && on_finish)
    {
        length_       = length;
        on_calculate_ = on_calculate;
        on_finish_    = on_finish;
        return *this;
    }

    bool is_running()
    {
        bool should_end = std::chrono::high_resolution_clock::now() > end_;
        if (should_end)
        {
            std::invoke(on_finish_);
            on_finish_ = [](){};
        }

        return not should_end;
    }
    theme_animation& start()      { end_ = std::chrono::high_resolution_clock::now() + length_; return *this; }
    theme_animation& calculate()  { fps_->calculate(); std::invoke(on_calculate_); return *this; }
    std::unique_ptr<fps>* get_fps() { return &fps_; }
};

} // namespace game

#endif // THEME_ANIMATION_HPP_
