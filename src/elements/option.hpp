#ifndef ELEMENTS_OPTION_HPP_
#define ELEMENTS_OPTION_HPP_

#include "../basic_headers.hpp"
#include "text.hpp"
#include "option_group.hpp"

namespace game::element_types
{

class option : public text
{
    friend option_group<option>;
    std::string option_group_name_;
    bool selected_ = false;
    std::shared_ptr<option_group<option>> group_ref_ = nullptr;
    pixel amp_size_original_ = 0;
    pixel amp_size_selected_ = 0;
    constexpr static pixel size_add_ = 50;

    void select()
    {
        if (not selected_)
        {
            amplify_to (amp_size_selected_);
            selected_ = true;
        }
    }

    void unselect()
    {
        if (selected_)
        {
            amplify_to (amp_size_original_);
            selected_ = false;
        }
    }

public:
    option(SDL_Renderer *r,
           std::string_view name,
           std::unordered_map<std::string, std::unique_ptr<element>> &a,
           camera &c): text {r, name, a, c} {}

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        text::build_from_toml(table);
        amp_size_original_ = state_.dest_.w;
        amp_size_selected_ = amp_size_original_ + size_add_;
        flag_ = flag::ghost;
        option_group<option>::tidy_up();

        assert(table->contains("option_group_name"));
        option_group_name_ = *table->get_as<std::string>("option_group_name");

        auto && reg = option_group<option>::registry();
        if (auto it = reg.find(option_group_name_); it == reg.end())
        {
            group_ref_ = std::make_shared<option_group<option>>();
            std::weak_ptr<option_group<option>> ptr = group_ref_;
            reg.emplace(option_group_name_, ptr);
        }
        else
            group_ref_ = it->second.lock();

        group_ref_->add(*this);
    }

    bool is_selected() { return selected_; }
};

} // namespace game::element_types

#endif // ELEMENTS_OPTION_HPP_
