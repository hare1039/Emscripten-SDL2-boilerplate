#ifndef KEYBOARD_STATE_HPP_
#define KEYBOARD_STATE_HPP_

#include "basic_headers.hpp"
#include <unordered_map>

namespace game
{

template<typename RelatedEnum, int StartEnum = 0>
class keyboard_state
{
    std::unordered_map<SDL_Keycode, bool> record_;
    std::unordered_map<RelatedEnum, SDL_Keycode> enum_map_;
    std::unordered_map<RelatedEnum, bool&> related_bool_;
    static_assert(std::is_enum_v<RelatedEnum>,
                  "RelatedEnum requires to be an enum, starts at 'StartEnum', increase by 1");

    inline
    void sync_bind_bool()
    {
        for (auto && pair : related_bool_)
            pair.second = record_.at(enum_map_.at(pair.first));
    }

public:
    template<int Start = StartEnum, typename ... MemKeys> inline
    keyboard_state& watch(SDL_Keycode key, MemKeys ... keys)
    {
        record_.emplace(key, false);
        enum_map_.emplace(static_cast<RelatedEnum>(Start), key);
        if constexpr (sizeof...(keys) > 0)
            watch<Start + 1>(std::forward<MemKeys>(keys)...);
        return *this;
    }

    template<typename ... Rest>
    keyboard_state& bind_bool(RelatedEnum key, bool &state, Rest&& ... rest)
    {
        related_bool_.emplace(key, state);
        if constexpr (sizeof...(Rest) > 0)
            bind_bool(std::forward<Rest>(rest)...);
        return *this;
    }

    void on_key_down(SDL_Keycode const & key)
    {
        for (auto && p : record_)
            if (key == p.first)
                p.second = true;
        sync_bind_bool();
    }

    void on_key_up(SDL_Keycode const & key)
    {
        for (auto && p : record_)
            if (key == p.first)
                p.second = false;
        sync_bind_bool();
    }

    inline
    bool is_pressing(RelatedEnum const & key) const
    {
        auto it = record_.find(enum_map_.at(key));
        return (it == record_.end())? false: it->second;
    }
};

} // namespace game

#endif // KEYBOARD_STATE_HPP_
