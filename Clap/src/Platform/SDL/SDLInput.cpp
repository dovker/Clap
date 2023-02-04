#include "pch.h"

#include "Input/Input.h"
#include "Core/Window.h"

#include "SDL3/SDL.h"

namespace Clap
{

    bool Input::IsKeyPressed(int keycode)
    {
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        return state[keycode];
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        float x, y;
        uint32_t buttons = SDL_GetMouseState(&x, &y);
        return buttons & SDL_BUTTON(button);
    }

    float Input::GetMouseX()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return x;
    }

    std::pair<float, float> Input::GetMousePos()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return {x, y };
    }

    float Input::GetMouseY()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return y;
    }
}