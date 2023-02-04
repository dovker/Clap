#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Events/Event.h"
#include "Core/Window.h"

union SDL_Event;

namespace Clap {

	class ImGuiRenderer
	{
	public:
        static void Init(Window* window);
		static void Begin();
		static void End();
        static void Shutdown();

        static void SetEventBlocking(bool blocking);
		static bool HandleSDLEvent(SDL_Event* e);
	private:
		static bool m_BlockEvents;
	};

}