#ifndef THEMES_STAGE_HPP_
#define THEMES_STAGE_HPP_

namespace game::theme_types
{

class stage : public theme
{
public:
    stage(SDL_Renderer * r, std::string path): theme{r, path} {}

    void on_key_down(SDL_Keycode const & key, Uint16 const &) override final
    {
        switch (key)
        {
        case SDLK_UP:
            elements["yoshi"]->jump();
            break;

        case SDLK_DOWN:
            for (auto && p : elements)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme = std::make_unique<stage>(renderer, "./asset/theme/01.toml");
            break;

        case SDLK_LEFT:
            elements["yoshi"]->move_left = true;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = true;
            break;

        case SDLK_SPACE:
            elements["yoshi"]->jump();
            elements["rin"]->dest.x += 100;
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &) override
    {
        switch (key)
        {
        case SDLK_LEFT:
            elements["yoshi"]->move_left = false;
            break;

        case SDLK_RIGHT:
            elements["yoshi"]->move_right = false;
            break;

        default:
            break;
        }
    }
};

} // namespace game

#endif // THEMES_STAGE_HPP_
