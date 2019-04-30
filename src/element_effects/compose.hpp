#ifndef ELEMENT_EFFECTS_COMPOSE_HPP_
#define ELEMENT_EFFECTS_COMPOSE_HPP_

#include <tuple>

namespace game::elements::effects
{

// concept: Animations = (time duration, Callable) + (time duration, Callable) ...
template<typename ... Animations>
class compose
{
    static inline auto now() { return std::chrono::high_resolution_clock::now(); }

    std::tuple<Animations ...> animations_;
    std::chrono::high_resolution_clock::time_point start_ {now()};

    template<int index>
    void run()
    {
        if constexpr (index < sizeof...(Animations))
        {
            if (now() < (std::get<index>(animations_) + start_))
                std::invoke(std::get<index + 1>(animations_));
            else
                run<index + 2>();
        }
    }

    template<int index, typename TimeDuration>
    void build_time_duration(TimeDuration&& prev)
    {
        if constexpr (index < sizeof...(Animations))
        {
            std::get<index>(animations_) += prev;
            build_time_duration<index + 2>(std::get<index>(animations_));
        }
    }

    template<int index>
    void show_impl()
    {
        if constexpr (index < sizeof...(Animations))
        {
            std::cout << index/2 << ": " << std::get<index>(animations_).count() << "\n";
            show_impl<index + 2>();
        }
    }

public:
    compose(Animations && ... animations): animations_ {std::forward_as_tuple(animations ...)}
    {
        using namespace std::chrono_literals;
        build_time_duration<0>(0ms);
    }

    void show() { show_impl<0>(); }

    void operator() () { run<0>(); }
};

} // namespace game::elements::effects

#endif // ELEMENT_EFFECTS_COMPOSE_HPP_
