#ifndef EVENT_HPP_
#define EVENT_HPP_
#pragma once

#include "basic_headers.hpp"

namespace game
{

/// this class is used for avoiding the giant switch when
/// handling events
class event
{
public:
    event() = default;
    virtual ~event() = default;

    virtual void on_key_down(SDL_Keycode const &, Uint16 const &) {}
    virtual void on_key_up  (SDL_Keycode const &, Uint16 const &) {}
    virtual void on_mouse_move(int  x      [[maybe_unused]], int y    [[maybe_unused]],
                               int  xrel   [[maybe_unused]], int yrel [[maybe_unused]],
                               bool left   [[maybe_unused]],
                               bool right  [[maybe_unused]],
                               bool middle [[maybe_unused]]) {}
    virtual void on_mouse_right_button_down (int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_mouse_right_button_up   (int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_mouse_left_button_down  (int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_mouse_left_button_up    (int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_mouse_middle_button_down(int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_mouse_middle_button_up  (int x [[maybe_unused]], int y [[maybe_unused]]) {}
    virtual void on_resize(int w [[maybe_unused]], int h [[maybe_unused]]) {}
    virtual void on_exit() {}
    virtual void on_user(Uint8 type  [[maybe_unused]],
                         int   code  [[maybe_unused]],
                         void* data1 [[maybe_unused]],
                         void* data2 [[maybe_unused]]) {}

    virtual void on_event(SDL_Event const & e)
    {
        // https://wiki.libsdl.org/SDL_Event#table
        switch (e.type)
        {
        case SDL_KEYDOWN:
            on_key_down(e.key.keysym.sym, e.key.keysym.mod);
            break;

        case SDL_KEYUP:
            on_key_up(e.key.keysym.sym, e.key.keysym.mod);
            break;

        case SDL_MOUSEMOTION:
            on_mouse_move(e.motion.x, e.motion.y,
                          e.motion.xrel, e.motion.yrel,
                          e.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT),
                          e.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT),
                          e.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    on_mouse_left_button_down(e.button.x, e.button.y);
                    break;
                case SDL_BUTTON_RIGHT:
                    on_mouse_right_button_down(e.button.x, e.button.y);
                    break;
                case SDL_BUTTON_MIDDLE:
                    on_mouse_middle_button_down(e.button.x, e.button.y);
                    break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    on_mouse_left_button_up(e.button.x, e.button.y);
                    break;
                case SDL_BUTTON_RIGHT:
                    on_mouse_right_button_up(e.button.x, e.button.y);
                    break;
                case SDL_BUTTON_MIDDLE:
                    on_mouse_middle_button_up(e.button.x, e.button.y);
                    break;
            }
            break;

        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SHOWN:
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                break;
            case SDL_WINDOWEVENT_MOVED:
                break;
            case SDL_WINDOWEVENT_RESIZED:
                on_resize(e.window.data1, e.window.data2);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                break;
            case SDL_WINDOWEVENT_RESTORED:
                break;
            case SDL_WINDOWEVENT_ENTER:
                // Mouse entered
                break;
            case SDL_WINDOWEVENT_LEAVE:
                // Mouse left
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                break;
            case SDL_WINDOWEVENT_CLOSE:
                break;
            default:
                break;
            }
            break;
        case SDL_QUIT:
            on_exit();
            break;

        case SDL_USEREVENT:
            on_user(e.user.type, e.user.code, e.user.data1, e.user.data2);
            break;

        default:
            break;
        }
    }
};

} // namespace game

#endif // EVENT_HPP_
