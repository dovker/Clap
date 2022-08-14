#include "pch.h"

#include "Core/Window.h"
#include "Core/Log.h"
#include "SDLOpenGLContext.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "glad/glad.h"

namespace Clap {

    SDLOpenGLContext::SDLOpenGLContext(Window* window)
    {
        m_Window = window->GetWindowPtr();
        #ifdef CLAP_USE_OPENGL_4_5
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        #endif

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // Create a OpenGL context on SDL2
        m_Context = SDL_GL_CreateContext((SDL_Window*)m_Window);

        // Load GL extensions using glad
        if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
            CLAP_ERROR("Failed to initialize the OpenGL context.");
            exit(1);
        }

        // Loaded OpenGL successfully.
        CLAP_INFO("OpenGL version loaded: ", GLVersion.major, ".", GLVersion.minor);
    }

    SDLOpenGLContext::~SDLOpenGLContext()
    {
        SDL_GL_DeleteContext(m_Context);
    }

    void SDLOpenGLContext::SetSwapInterval(bool interval)
    {
        SDL_GL_SetSwapInterval(interval);
    }

    void SDLOpenGLContext::SwapBuffers()
    {
        SDL_GL_SwapWindow((SDL_Window*)m_Window);
    }

    void SDLOpenGLContext::ForceModernOpenGL()
    {
        #ifdef CLAP_PLATFORM_MACOS
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        #endif
    }
}