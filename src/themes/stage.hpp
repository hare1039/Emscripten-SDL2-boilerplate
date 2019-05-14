#ifndef THEMES_STAGE_HPP_
#define THEMES_STAGE_HPP_

namespace game::theme_types
{

class stage : public theme
{
public:
    stage(SDL_Renderer * r, std::unique_ptr<fps>* gfps, std::string path):
        theme {r, gfps, path} {}

    void on_key_down(SDL_Keycode const & key, Uint16 const &mod) override final
    {
        elements_["yoshi"]->on_key_down(key, mod);
        switch (key)
        {
        case SDLK_DOWN:
            for (auto && p : elements_)
                std::cout << p.first << " -> " << p.second.get() << std::endl;
            next_theme_ = std::make_unique<stage>(renderer_, game_fps_, "./asset/theme/01.toml");
            break;

        default:
            break;
        }
    }

    void on_key_up  (SDL_Keycode const & key, Uint16 const &mod) override
    {
        elements_["yoshi"]->on_key_up(key, mod);
    }
};

} // namespace game

#endif // THEMES_STAGE_HPP_
