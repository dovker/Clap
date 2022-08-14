#pragma once

#include "Core/Window.h"
#include "Graphics/API/Context.h"

struct SDL_Window;

namespace Clap {

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowSpecification& properties);
		virtual ~SDLWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline double GetTime() override;
		inline double GetFrequency() override;

		virtual void SetVSync(bool enabled)             override;
        virtual void SetFullscreen(bool enabled)        override;
        virtual void SetCursorLock(bool enabled)        override;
        virtual void SetCursorVisible(bool enabled)     override;
        virtual void SetTitle(const std::string& name)  override;

		virtual void StartTextInput() override;
		virtual void StopTextInput()  override;

        virtual void PollEvents() override;

		bool IsVSync() const override;
		void* GetWindowPtr() override { return m_Window; }
	private:
		SDL_Window* m_Window;
        
        GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync, Fullscreen = false;
		};

		WindowData m_Data;
	};

}
