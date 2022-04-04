#include "pch.h"

#include "Core/Log.h"
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/EventSystem.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "SDLWindow.h"
#include "SDLOpenGLContext.h"

namespace Clap {

	Window* Window::Create(const WindowSpecification& props)
	{
		return new SDLWindow(props);
	}

	SDLWindow::SDLWindow(const WindowSpecification& properties)
	{
		m_Data.Title = properties.Title;
        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            CLAP_ERROR("SDL2 video subsystem couldn't be initialized. Error: ", SDL_GetError());
            exit(1);
        }
        uint32_t GraphicsFlag;
        #if defined(CLAP_USE_OPENGL)
            GraphicsFlag = SDL_WINDOW_OPENGL;
            #if defined(CLAP_PLATFORM_MACOS) 
            SDLOpenGLContext::ForceModernOpenGL();
            #endif
        #endif

        auto flags = SDL_WINDOW_SHOWN | GraphicsFlag;

        m_Window = SDL_CreateWindow(properties.Title.c_str(),
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            properties.Width, properties.Height, flags);


        #ifdef CLAP_USE_OPENGL
            m_Context = new SDLOpenGLContext(this);
        #endif

        SetVSync(true);
	}

	SDLWindow::~SDLWindow()
	{
        delete m_Context;
		SDL_DestroyWindow(m_Window);

        SDL_Quit();
	}

    void SDLWindow::PollEvents()
    {
		// GamepadButtonPressed, GamepadButtonReleased, GamepadAxisMoved
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            Event e;
            e.Immediate = false;
            switch(event.type) {
                case SDL_QUIT:
                    e.Type = EventType::WindowClose;
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.type)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            e.Type = EventType::WindowResize;
                            e.Data.WindowResizeEvent = { event.window.data1, event.window.data2 };
                        break;
                        case SDL_WINDOWEVENT_CLOSE:
                            e.Type = EventType::WindowClose;
                        break;
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            e.Type = EventType::WindowFocus;
                        break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            e.Type = EventType::WindowLostFocus;
                        break;
                    }
                break;

                case SDL_MOUSEMOTION:
                    e.Type = EventType::MouseMoved;
                    e.Data.MouseEvent = { (float)event.motion.x, (float)event.motion.y };
                break;

                case SDL_MOUSEWHEEL:
                    e.Type = EventType::MouseScrolled;
                    e.Data.MouseEvent = { (float)event.wheel.x, (float)event.wheel.y };
                break;

                case SDL_MOUSEBUTTONDOWN:
                    e.Type = EventType::MouseButtonPressed;
                    e.Data.MouseButtonEvent = { event.button.button };
                break;

                case SDL_MOUSEBUTTONUP:
                    e.Type = EventType::MouseButtonReleased;
                    e.Data.MouseButtonEvent = { event.button.button };
                break;

                case SDL_KEYDOWN:
                    e.Type = EventType::KeyPressed;
                    e.Data.KeyEvent = { event.key.keysym.scancode, event.key.repeat };
                break;

                case SDL_KEYUP:
                    e.Type = EventType::KeyReleased;
                    e.Data.KeyEvent = { event.key.keysym.scancode, event.key.repeat };
                break;
            }
            EventSystem::RegisterEvent(e);
        }
        //TODO: CONTROLLER SUPPORT
    }

	void SDLWindow::SetFullscreen(bool enabled) 
	{ 
		if(m_Data.Fullscreen != enabled)
        {
            m_Data.Fullscreen = enabled;
            SDL_SetWindowFullscreen(m_Window, enabled);
        }
	} 
    void SDLWindow::SetCursorVisible(bool enabled)
    {
        SDL_ShowCursor(enabled ? SDL_ENABLE : SDL_DISABLE);
    }
    void SDLWindow::SetCursorLock(bool enabled) 
	{ 
		SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
	} 
    void SDLWindow::SetVSync(bool enabled)
	{
		m_Context->SetSwapInterval(enabled);
		m_Data.VSync = enabled;
	}
    void SDLWindow::SetTitle(const std::string& name)
    {
        SDL_SetWindowTitle(m_Window, name.c_str());
    }

	void SDLWindow::OnUpdate()
	{
		PollEvents();
        m_Context->SwapBuffers();
	}

	inline double SDLWindow::GetTime()
	{
		return (double)SDL_GetPerformanceCounter();
	}

	bool SDLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
