#ifndef ELEMENTS_TEXT_HPP_
#define ELEMENTS_TEXT_HPP_

#include <cassert>
#include "../basic_headers.hpp"
#include "../element.hpp"

namespace game::element_types
{

class text : public floating
{
public:
    std::string ttf_path_;
    std::string text_;
    SDL_Color   color_{0, 0, 0, 0};
    unsigned int font_size_ = 16;
public:
    text(SDL_Renderer *r,
         std::string_view name,
         std::unordered_map<std::string, std::unique_ptr<element>> &a,
         camera &c): floating {r, name, a, c}
    {
        type_ = type::text;
    }

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        build_from_toml_basic(table);
        flag_ = flag::ghost;
        assert(table->contains("ttf_path"));

        ttf_path_ = *table->get_as<std::string>("ttf_path");
        text_     =  table->get_as<std::string>("text").value_or(text_);

        state_.dest_.w  = table->get_as<pixel>("w").value_or(state_.dest_.w);
        state_.dest_.h  = table->get_as<pixel>("h").value_or(state_.dest_.h);

        using color_int = decltype(color_.r);
        color_ = SDL_Color {
            table->get_as<color_int>("color_red")  .value_or(color_.r),
            table->get_as<color_int>("color_green").value_or(color_.g),
            table->get_as<color_int>("color_blue") .value_or(color_.b),
            table->get_as<color_int>("color_alpha").value_or(color_.a)
        };
        font_size_ = table->get_as<unsigned int>("font_size").value_or(font_size_);
        regenerate_texture();
    }

    void update_text(std::string_view new_text,
                     std::optional<unsigned int> font_size = std::nullopt,
                     std::optional<SDL_Color> color = std::nullopt)
    {
        text_      = new_text;
        font_size_ = font_size.value_or(font_size_);
        color_     = color.value_or(color_);
        regenerate_texture();
    }

private:
    error_code regenerate_texture()
    {
        TTF_Font_ptr font_ {TTF_OpenFont(ttf_path_.c_str(), font_size_), &TTF_CloseFont};

        SDL_Surface_ptr font_surface {TTF_RenderText_Blended(font_.get(), text_.c_str(), color_), &SDL_FreeSurface};

        if (font_surface == nullptr)
        {
            std::cout << TTF_GetError() << std::endl;
            return -1;
        }

        return set_texture(std::move(font_surface), state_.dest_.w, state_.dest_.h, animation::rotate_type::none);
    }
};

} // namespace game::element_types

#endif // ELEMENTS_TEXT_HPP_
