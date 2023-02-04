#include "pch.h"

#include "Core/Log.h"
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/EventSystem.h"

#include "Graphics/API/API.h"

#define SDL_MAIN_HANDLED
#include "SDL3/SDL.h"

#include "SDLWindow.h"
#include "SDLOpenGLContext.h"

#include "Graphics/ImGui/ImGuiRenderer.h"

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
            CLAP_ERROR("SDL3 video subsystem couldn't be initialized. Error: ", SDL_GetError());
            exit(1);
        }
        uint32_t GraphicsFlag = 0;
        #if defined(CLAP_USE_OPENGL)
            GraphicsFlag = SDL_WINDOW_OPENGL;
        #endif


        auto flags = SDL_WINDOW_RESIZABLE | GraphicsFlag;

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
            e.Handled   = ImGuiRenderer::HandleSDLEvent(&event);
            switch(event.type) {
                case SDL_EVENT_QUIT:
                    e.Type = EventType::WindowClose;
                    e.Handled = false;
                break;
                
                case SDL_EVENT_WINDOW_RESIZED:
                    m_Data.Width = event.window.data1;
                    m_Data.Height = event.window.data2;

                    e.Type = EventType::WindowResize;
                    e.Data.WindowResizeEvent = { event.window.data1, event.window.data2 };

                    //e.Handled = false;
                break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    e.Type = EventType::WindowClose;
                    e.Handled = false;
                break;

                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                    e.Type = EventType::WindowFocus;
                    e.Handled = false;
                break;

                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    e.Type = EventType::WindowLostFocus;
                    e.Handled = false;
                break;

                case SDL_EVENT_MOUSE_MOTION:
                    e.Type = EventType::MouseMoved;
                    e.Data.MouseEvent = { (float)event.motion.x, (float)event.motion.y };
                break;

                case SDL_EVENT_MOUSE_WHEEL:
                    e.Type = EventType::MouseScrolled;
                    e.Data.MouseEvent = { (float)event.wheel.x, (float)event.wheel.y };
                break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    e.Type = EventType::MouseButtonPressed;
                    e.Data.MouseButtonEvent = { event.button.button };
                break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    e.Type = EventType::MouseButtonReleased;
                    e.Data.MouseButtonEvent = { event.button.button };
                    e.Handled = false;
                break;

                case SDL_EVENT_KEY_DOWN:
                    e.Type = EventType::KeyPressed;
                    e.Data.KeyEvent = { event.key.keysym.scancode, event.key.repeat };
                break;

                case SDL_EVENT_KEY_UP:
                    e.Type = EventType::KeyReleased;
                    e.Data.KeyEvent = { event.key.keysym.scancode, event.key.repeat };
                    e.Handled = false;
                break;

                case SDL_EVENT_TEXT_INPUT:
                    e.Type = EventType::KeyTyped;
                    e.Data.KeyTypedEvent = { event.text.text[0] };
                break;
            }
            EventSystem::RegisterEvent(e);
        }
        //TODO: CONTROLLER SUPPORT
    }

	void SDLWindow::SetFullscreen(bool enabled) 
	{ 
        m_Data.Fullscreen = enabled;
        SDL_SetWindowFullscreen(m_Window, (SDL_bool)enabled);
	} 
    void SDLWindow::SetCursorVisible(bool enabled)
    {
        if(enabled)
            SDL_ShowCursor();
        else SDL_HideCursor();
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

    void SDLWindow::StartTextInput() 
    {
        SDL_StartTextInput();
    }

    void SDLWindow::StopTextInput()  
    {
        SDL_StopTextInput();
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

    inline double SDLWindow::GetFrequency()
	{
		return (double)SDL_GetPerformanceFrequency();
	}

    uint32_t SDLWindow::GetWidthPixels() const
    {
        int w, h;
        SDL_GetWindowSizeInPixels(m_Window, &w, &h);
        return w;
    }

    uint32_t SDLWindow::GetHeightPixels() const
    {
        int w, h;
        SDL_GetWindowSizeInPixels(m_Window, &w, &h);
        return h;
    }

    double SDLWindow::GetDPIScale() const
    {
        return (double)GetHeightPixels() / (double)GetHeight();
    }


	bool SDLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
