#ifndef ELEMENT_EFFECTS_BASE_HPP_
#define ELEMENT_EFFECTS_BASE_HPP_

#include <chrono>
#include "utility.hpp"
#include "../element.hpp"

namespace game::elements::effects
{

namespace {
    using namespace std::literals;
}

class base
{
protected:
    std::chrono::high_resolution_clock::duration length_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    bool is_called = false;
    static inline auto now() { return std::chrono::high_resolution_clock::now(); }

public:
    base(std::chrono::high_resolution_clock::duration length):
        length_{length} {}

    std::chrono::high_resolution_clock::duration length() { return length_; }
    void reset() { start_ = now(); }

    virtual
    void operator() (element &)
    {
        if (not is_called)
        {
            is_called = true;
            start_ = now();
        }
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_BASE_HPP_
