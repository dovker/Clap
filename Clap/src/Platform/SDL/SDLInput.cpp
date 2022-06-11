#include "pch.h"

#include "Input/Input.h"
#include "Core/Window.h"

#include "SDL.h"

namespace Clap
{

    bool Input::IsKeyPressed(int keycode)
    {
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        return state[keycode];
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        int x, y;
        uint32_t buttons = SDL_GetMouseState(&x, &y);
        return buttons & SDL_BUTTON(button);
    }

    float Input::GetMouseX()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return (float)x;
    }

    std::pair<float, float> Input::GetMousePos()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return {(float)x, (float)y };
    }

    float Input::GetMouseY()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return (float)y;
    }
}