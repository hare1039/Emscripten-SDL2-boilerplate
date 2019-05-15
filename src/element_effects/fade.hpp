#ifndef ELEMENT_EFFECTS_FADE_HPP_
#define ELEMENT_EFFECTS_FADE_HPP_

#include "base.hpp"
#include <optional>

namespace game::elements::effects
{

class fade : public base
{
public:
    fade(std::chrono::high_resolution_clock::duration length):
        base(length) {}

    void calculate (element &e) override
    {
        e.set_alpha((1 - progress()) * 255);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_FADE_HPP_
