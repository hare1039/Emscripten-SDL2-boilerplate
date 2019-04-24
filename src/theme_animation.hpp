#ifndef THEME_ANIMATION_HPP_
#define THEME_ANIMATION_HPP_

#include "basic_headers.hpp"
#include "fps.hpp"
#include <memory>
#include <chrono>

namespace game
{

namespace {
    using namespace std::literals;
}

class theme_animation
{
    std::chrono::milliseconds length_;
    std::chrono::time_point<std::chrono::steady_clock> end_ {std::chrono::steady_clock::now() + 24h};
    std::function<void(void)> work_;
    std::unique_ptr<fps>      fps_ {std::make_unique<fps>()};
public:

    void set (std::chrono::milliseconds length, std::function<void(void)> && work)
    {
        length_ = length;
        work_   = work;
    }
    bool is_running() { return std::chrono::steady_clock::now() > end_; }
    void start()      { end_ = std::chrono::steady_clock::now() + length_; }
    void calculate()  { fps_->calculate(); work_(); }
    std::unique_ptr<fps>* get_fps() { return &fps_; }
};

} // namespace game

#endif // THEME_ANIMATION_HPP_
