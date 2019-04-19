#ifndef ELEMENTS_OPTION_GROUP_HPP_
#define ELEMENTS_OPTION_GROUP_HPP_

#include "../basic_headers.hpp"
#include <unordered_map>

namespace game::element_types
{

class option;
template<typename Option = option>
class option_group : std::enable_shared_from_this<option_group<Option>>
{
    std::vector<Option *> options_;
public:
    void add (Option & o)
    {
        options_.push_back(&o);
    }

    void select (Option & selected)
    {
        for (Option * o : options_)
            o->unselect();

        selected.select();
    }

public:
    static
    auto& registry()
    {
        static std::unordered_map<std::string, std::weak_ptr<option_group>> reg;
        return reg;
    }

    static
    void tidy_up()
    {
        for (auto it = registry().begin(); it != registry().end();)
        if (it->second.lock() == nullptr)
            it = registry().erase(it);
        else
            ++it;
    }
};

} // namespace game::element_types

#endif // ELEMENTS_OPTION_GROUP_HPP_
