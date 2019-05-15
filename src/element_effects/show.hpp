#ifndef ELEMENT_EFFECTS_SHOW_HPP_
#define ELEMENT_EFFECTS_SHOW_HPP_

#include "base.hpp"
#include <optional>

namespace game::elements::effects
{

class show : public base
{
public:
    show(std::chrono::high_resolution_clock::duration length):
        base(length) {}

    void calculate (element &e) override
    {
        e.set_alpha(progress() * 255);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_SHOW_HPP_
