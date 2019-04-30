#ifndef ELEMENT_EFFECTS_UTILITY_HPP_
#define ELEMENT_EFFECTS_UTILITY_HPP_

#include <chrono>

namespace game::elements::effects
{

namespace {
    using namespace std::literals;
}

template <typename T, typename ... Args> constexpr inline
T make(Args&& ... args) { return game::utility::make_object<T, Args...>(std::forward<Args>(args)...); }

}

#endif // ELEMENT_EFFECTS_UTILITY_HPP_
