#ifndef DECLTIME_HPP_
#define DECLTIME_HPP_

#include <chrono>

namespace game::chrono
{

using namespace std::chrono_literals;
using duration   = std::chrono::high_resolution_clock::duration;
using time_point = std::chrono::high_resolution_clock::time_point;

inline auto now() { return std::chrono::high_resolution_clock::now(); }

} // namespace game::chrono

// this namespace import std::literals::chrono_literals
namespace game::detail
{

using namespace std::literals::chrono_literals;

}

#endif // DECLTIME_HPP_
