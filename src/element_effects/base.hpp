#ifndef ELEMENT_EFFECTS_BASE_HPP_
#define ELEMENT_EFFECTS_BASE_HPP_

#include <chrono>
#include "../element.hpp"

namespace game::elements::effects
{

namespace {
    using namespace std::literals;
}

template <typename T, typename ... Args> constexpr inline
T make(Args&& ... args) { T o(std::forward<Args>(args)...); return o; }

class base
{
protected:
    std::chrono::high_resolution_clock::duration length_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    inline auto now() { return std::chrono::high_resolution_clock::now(); }

public:
    base(std::chrono::high_resolution_clock::duration length):
        length_{length},
        start_{std::chrono::high_resolution_clock::now()} {}

    virtual void operator() (element &e) = 0;
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_BASE_HPP_
