#ifndef ELEMENT_HPP_
#define ELEMENT_HPP_
#pragma once

#include <string>
#include <memory>
#include <deque>
#include <algorithm>
#include "basic_headers.hpp"
#include "animation.hpp"
namespace game
{

class element
{
protected:
    SDL_Renderer   *renderer = nullptr;
    SDL_Rect        dest = {}, src = {};
    SDL_Texture_ptr texture {nullptr, &SDL_DestroyTexture};
    std::unique_ptr<animation> anime_info;
public:
    static
    std::deque<element *> & all_elements()
    {
        static std::deque<element *> _v;
        return _v;
    }


public:
    element(SDL_Renderer *r): renderer{r} { all_elements().push_back(this); }
    virtual ~element()
    {
        std::deque<element *> & all = all_elements();
        auto it = std::remove(all.begin(), all.end(), this);
        all.erase(it, all.end());
    }
    
    virtual
    error_code set_texture(std::string path, int pic_frame = 1)
    {
        SDL_Surface_ptr image {IMG_Load(path.c_str()), &SDL_FreeSurface};
        if (not image)
        {
            std::cout << IMG_GetError() << std::endl;
            return -1;
        }

        texture.reset (SDL_CreateTextureFromSurface (renderer, image.get()));
        src.w   = dest.w = image->w;
        src.h   = dest.h = image->h / pic_frame;
        if (pic_frame != 1)
            anime_info->oscillate = true;

        anime_info.reset (new animation(pic_frame));

        return 0;
    }

    virtual
    void handle_event(SDL_Event &) {}

    
    virtual
    void calculate()
    {
        anime_info->animate();
        src.y = src.w * anime_info->get_current_frame();
    }

    virtual
    void render()
    {
        SDL_RenderCopy (renderer, texture.get(), &src, &dest);
    }
};

using element_ptr = std::unique_ptr<element>;

}// namespace game
#endif // ELEMENT_HPP_
