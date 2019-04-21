#ifndef THEMES_STAGE_HPP_
#define THEMES_STAGE_HPP_

namespace game::theme_types
{

class stage : public theme
{
public:
    stage(SDL_Renderer * r, std::unique_ptr<fps>* gfps, std::string path):
        theme {r, gfps, path} {}

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override final
    {
        auto yoshi = dynamic_cast<element_types::player *>(elements["yoshi"].get());
        switch (key)
        {
        case SDLK_UP:
            yoshi->jump();
            break;

        case SDLK_DOWN:
            for (auto && p : elements)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme = std::make_unique<stage>(renderer, game_fps, "./asset/theme/01.toml");
            break;

        case SDLK_LEFT:
            yoshi->move_left_ = true;
            break;

        case SDLK_RIGHT:
            yoshi->move_right_ = true;
            break;

        case SDLK_SPACE:
            yoshi->jump();
            elements["rin"]->state_.dest_.x += 100;
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        auto yoshi = dynamic_cast<element_types::player *>(elements["yoshi"].get());
        switch (key)
        {
        case SDLK_LEFT:
            yoshi->move_left_ = false;
            break;

        case SDLK_RIGHT:
            yoshi->move_right_ = false;
            break;

        default:
            break;
        }
    }
};

} // namespace game

#endif // THEMES_STAGE_HPP_
