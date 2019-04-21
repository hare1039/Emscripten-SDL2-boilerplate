#ifndef ELEMENTS_OPTION_HPP_
#define ELEMENTS_OPTION_HPP_

#include "../basic_headers.hpp"
#include "text.hpp"
#include "../utility.hpp"

namespace game::element_types
{

namespace {

template<typename IntegerType,
         std::enable_if_t<std::is_integral<IntegerType>::value, int> = 0>
SDL_Color operator + (SDL_Color const & org, IntegerType bright)
{
    return game::utility::operator + (org, bright);
}

}


class option : public text
{
public:
    class group : std::enable_shared_from_this<group>
    {
        std::vector<option *> options_;
    public:
        void add (option & o) { options_.push_back(&o); }

        void select (option & selected)
        {
            for (option * o : options_)
                o->unselect();

            selected.select();
        }

        option& next()
        {
            for (auto it = options_.begin(); it != options_.end(); it = std::next(it))
                if ((*it)->is_selected())
                    return **((std::next(it) == options_.end())? it: std::next(it));
            return *options_.front();
        }

        option& prev()
        {
            for (auto it = options_.rbegin(); it != options_.rend(); it = std::next(it))
                if ((*it)->is_selected())
                    return **((std::next(it) == options_.rend())? it: std::next(it));
            return *options_.back();
        }
        inline void select_next() { select(next()); }
        inline void select_prev() { select(prev()); }

    public:
        static
        auto& registry()
        {
            static std::unordered_map<std::string, std::weak_ptr<group>> reg;
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

        template<typename String> static inline
        void select_next(String&& group_name)
        {
            registry()[std::forward<String>(group_name)].lock()->select_next();
        }

        template<typename String> static inline
        void select_prev(String&& group_name)
        {
            registry()[std::forward<String>(group_name)].lock()->select_prev();
        }
    };
    friend group;

private:
    std::string group_name_;
    bool selected_ = false;
    std::shared_ptr<group> group_ref_ = nullptr;
    pixel amp_size_original_ = 0;
    pixel amp_size_selected_ = 0;
    constexpr static pixel size_add_ = 50;

    SDL_Color color_original_ {};
    SDL_Color color_selected_ {};
    constexpr static Uint8 color_add_ = 100; // typeof SDL_Color.a

    void select()
    {
        if (not selected_)
        {
            update_text(text_ + " <", std::nullopt, color_selected_);
            amplify_to (amp_size_selected_, std::nullopt, amplify_mode::top_left);
            selected_ = true;
        }
    }

    void unselect()
    {
        using namespace std::literals;
        if (selected_)
        {
            update_text(text_.substr(0, text_.size() - (" <"s.size())), std::nullopt, color_original_);
            amplify_to (amp_size_original_, std::nullopt, amplify_mode::top_left);
            selected_ = false;
        }
    }

public:
    option(SDL_Renderer *r,
           std::string_view name,
           cache_container<std::string, element> &a,
           camera &c,
           std::unique_ptr<fps>* game_fps): text {r, name, a, c, game_fps} {}

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        text::build_from_toml(table);
        amp_size_original_ = state_.dest_.w;
        amp_size_selected_ = amp_size_original_ + size_add_;
        color_original_    = color_;
        color_selected_    = color_ + color_add_;
        flag_ = flag::ghost;
        group::tidy_up();

        assert(table->contains("group_name"));
        group_name_ = *table->get_as<std::string>("group_name");
        if (table->get_as<bool>("selected").value_or(selected_))
            select();

        auto && reg = group::registry();
        if (auto it = reg.find(group_name_); it == reg.end())
        {
            group_ref_ = std::make_shared<group>();
            std::weak_ptr<group> ptr = group_ref_;
            reg.emplace(group_name_, ptr);
        }
        else
            group_ref_ = it->second.lock();

        group_ref_->add(*this);
    }

    bool is_selected() { return selected_; }
};

} // namespace game::element_types

#endif // ELEMENTS_OPTION_HPP_
