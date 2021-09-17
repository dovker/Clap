#include "pch.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glad/glad.h>

#include <glm/glm.hpp>

int main()
{
    uint32_t WindowFlags = SDL_WINDOW_OPENGL;
    SDL_Window *Window = SDL_CreateWindow("OpenGL Test", 0, 0, 1280, 720, WindowFlags);
    CLAP_ASSERT(Window, "Window not created!!");
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
    
    bool Running = true;
    bool FullScreen = false;
    while (Running)
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    Running = 0;
                    break;
                case 'f':
                    FullScreen = !FullScreen;
                    if (FullScreen)
                    {
                    SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else
                    {
                    SDL_SetWindowFullscreen(Window, WindowFlags);
                    }
                    break;
                default:
                    CLAP_TRACE("Key Pressed: ", Event.key.keysym.sym, "!");
                    break;
                }
            }
            else if (Event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }

        glViewport(0, 0, 1280, 720);
        glClearColor(1.f, 0.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(Window);
    }
    return 0;
}