#ifndef COMPILE_TIME_UTILITY_
#define COMPILE_TIME_UTILITY_

namespace game::utility
{

namespace compile_time {

namespace {

template <int expo, int base, typename IntegerType> constexpr
IntegerType pow_impl()
{
    static_assert(expo >= 0);
    if constexpr (expo == 0)
        return 1;
    else
        return base * pow_impl<expo - 1, base, IntegerType>();
}

} // namespace ()


template <int expo, int base = 2, typename IntegerType = unsigned long long int> constexpr
IntegerType pow = pow_impl<expo, base, IntegerType>();

} // namespace compile_time

namespace type_traits
{

template <typename, typename = std::void_t<>>
struct is_rectangle : std::false_type {};

template <typename T>
struct is_rectangle<T,
                    std::void_t<decltype(std::declval<T>().x),
                                decltype(std::declval<T>().y),
                                decltype(std::declval<T>().w),
                                decltype(std::declval<T>().h)>
                    > : std::true_type {};

template <typename, typename = std::void_t<>>
struct is_point : std::false_type {};

template <typename T>
struct is_point<T,
                std::void_t<decltype(std::declval<T>().x),
                            decltype(std::declval<T>().y)>
                > : std::true_type {};

} // namespace type_traits

} // namespace game::utility

#endif // COMPILE_TIME_UTILITY_
