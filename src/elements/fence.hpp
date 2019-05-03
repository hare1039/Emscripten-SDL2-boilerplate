#ifndef ELEMENTS_FENCE_HPP_
#define ELEMENTS_FENCE_HPP_

#include "obstacle.hpp"

namespace game::elements::types
{

class fence : public obstacle
{
public:
    fence(SDL_Renderer *r,
          std::string_view name,
          cache_container<std::string, element> &a,
          camera &c,
          std::unique_ptr<fps>* game_fps): obstacle {r, name, a, c, game_fps}
    {
        hardness_ = hardness::air;
        type_     = type::fence;
    }
};

} // namespace game::elements::types

#endif // ELEMENTS_FENCE_HPP_
