#ifndef ELEMENT_EFFECTS_BASE_HPP_
#define ELEMENT_EFFECTS_BASE_HPP_

#include <chrono>
#include "utility.hpp"
#include "../element.hpp"

namespace game::elements::effects
{

class base
{
protected:
    chrono::duration length_;
    chrono::time_point start_;
    bool is_called = false;

    virtual void on_start (element &) {}
    virtual void calculate(element &) {}

    inline
    double progress()
    {
        double percent = (chrono::now() - start_).count() / static_cast<double>(length_.count());
        if (percent <= 0)
            return 0;
        else if (percent >= 1)
            return 1;
        else
            return percent;
    }

public:
    base(std::chrono::high_resolution_clock::duration length):
        length_{length} {}

    std::chrono::high_resolution_clock::duration length() { return length_; }

    void operator() (element &e)
    {
        if (not is_called)
        {
            is_called = true;
            start_ = chrono::now();
            on_start(e);
        }
        calculate(e);
    }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_BASE_HPP_
