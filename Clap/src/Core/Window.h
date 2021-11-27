#pragma once

namespace Clap
{
    struct WindowSpecification
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Clap Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class  Window
	{
	public:
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual float GetTime() = 0;

		// Window attributes
		virtual void SetVSync(bool enabled) = 0;
        virtual void SetFullscreen(bool enabled) = 0;
        virtual void SetCursorLock(bool enabled) = 0;
        virtual void SetTitle(const std::string& name) = 0;
        
		virtual bool IsVSync() const = 0;
		virtual void* GetWindowPtr() = 0;

		static Window* Create(const WindowSpecification& props = WindowProperties());
	};
}