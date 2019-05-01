#ifndef ELEMENT_EFFECTS_AMPLIFY_HPP_
#define ELEMENT_EFFECTS_AMPLIFY_HPP_

#include "base.hpp"
#include <optional>

namespace game::elements::effects
{

class amplify : public base
{
    pixel original_width_, expand_width_;
    double mutiple_;
public:
    amplify(std::chrono::high_resolution_clock::duration length, double mutiple):
        base {length},
        mutiple_ {mutiple} {}

    void on_start(element &e) override
    {
        original_width_ = e.state_.dest_.w;
        expand_width_   = (mutiple_ - 1) * original_width_;
    }

    void operator() (element &e) override
    {
        base::operator()(e);
        double percent = (now() - start_).count() / static_cast<double>(length_.count());
        e.amplify_to(expand_width_ * percent + original_width_);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_AMPLIFY_HPP_
