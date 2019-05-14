#include <iostream>

#include "app.hpp"

void run_loop (void * ptr)
{
    auto game_ptr = static_cast<game::app *>(ptr);
    try
    {
        game_ptr->run();
    } catch (std::exception const & e) {
        std::cout << "[run_loop] " << e.what() << std::endl;
    }
}

int main()
{
    constexpr int simulate_infinite_loop = 1;
    game::app game;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(run_loop, &game, -1, simulate_infinite_loop);
#else
    for (;;) { run_loop(&game); }
#endif // __EMSCRIPTEN__
}
