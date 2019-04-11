#ifndef ENUM_OPERATOR_
#define ENUM_OPERATOR_

#include <type_traits>
#include <iostream>
#include <bitset>
#include "basic_headers.hpp"
namespace game
{

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum operator &(Enum lhs, Enum rhs)
{
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum operator ^(Enum lhs, Enum rhs)
{
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) ^
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum operator |(Enum lhs, Enum rhs)
{
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum operator ~(Enum rhs)
{
    return static_cast<Enum> (
        ~static_cast<std::underlying_type_t<Enum>>(rhs)
    );
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum& operator |=(Enum &lhs, Enum rhs)
{
    lhs = static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );

    return lhs;
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum& operator &=(Enum &lhs, Enum rhs)
{
    lhs = static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );

    return lhs;
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
Enum& operator ^=(Enum &lhs, Enum rhs)
{
    lhs = static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) ^
        static_cast<std::underlying_type_t<Enum>>(rhs)
    );

    return lhs;
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
bool bool_of(Enum lhs)
{
    return !!static_cast<std::underlying_type_t<Enum>>(lhs);
}

template<typename Enum,
         std::enable_if_t<std::is_enum_v<Enum>, int> = 0>
std::ostream& operator << (std::ostream& os, Enum e)
{
    return os << std::bitset<sizeof(Enum)>(static_cast<std::underlying_type_t<Enum>>(e));
}

} // namespace game

#endif // ENUM_OPERATOR_
