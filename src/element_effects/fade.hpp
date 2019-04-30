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

    void operator() (element &e) override
    {
        base::operator()(e);
        double percent = (now() - start_).count() / static_cast<double>(length_.count());
        e.set_alpha((1 - percent) * 255);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_FADE_HPP_
