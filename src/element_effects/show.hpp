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

    void operator() (element &e) override
    {
        base::operator()(e);
        double percent = (now() - start_).count() / static_cast<double>(length_.count());
        if (0 <= percent and percent <= 1)
            e.set_alpha(percent * 255);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_SHOW_HPP_
