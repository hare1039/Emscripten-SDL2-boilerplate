#ifndef ELEMENTS_TEXT_HPP_
#define ELEMENTS_TEXT_HPP_

#include <cassert>
#include "../basic_headers.hpp"
#include "../element.hpp"

namespace game::elements::types
{

class text : public floating
{
public:
    std::string ttf_path_;
    std::string text_;
    SDL_Color   color_{0, 0, 0, 255};
    unsigned int font_size_ = 16;
public:
    text(std::string_view name,
         cache_container<std::string, element> &a,
         SDL_Renderer *r,
         camera &c,
         std::unique_ptr<fps>* game_fps): floating {name, a, r, c, game_fps}
    {
        type_     = type::text;
        hardness_ = hardness::air;
    }

    void build_from_toml(std::shared_ptr<cpptoml::table> table) override
    {
        build_from_toml_basic(table);
        flag_ = flag::ghost;
        assert(table->contains("ttf_path"));

        ttf_path_ = *table->get_as<std::string>("ttf_path");
        text_     =  table->get_as<std::string>("text").value_or(text_);

        using color_int = decltype(color_.r);
        color_ = SDL_Color {
            table->get_as<color_int>("color_red")  .value_or(color_.r),
            table->get_as<color_int>("color_green").value_or(color_.g),
            table->get_as<color_int>("color_blue") .value_or(color_.b),
            table->get_as<color_int>("color_alpha").value_or(color_.a)
        };
        font_size_ = table->get_as<unsigned int>("font_size").value_or(font_size_);
        regenerate_texture();
        if (auto rate = table->get_as<double>("amplify"); rate)
            amplify(*rate);
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
        pixel w = font_surface->w, h = font_surface->h;
        if (set_texture(std::move(font_surface), w, h, animation::rotate_type::none))
        {
            std::cout << "set_texture error" << std::endl;
            return -1;
        }

        return set_alpha(color_.a);
    }
};

} // namespace game::elements::types

#endif // ELEMENTS_TEXT_HPP_
