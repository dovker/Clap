#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Events/Event.h"

namespace Clap {

	class ImGuiLayer
	{
	public:
        static void Init();
        static void OnEvent(Event& e);
		static void Begin();
		static void End();
        static void Shutdown();

        static void SetEventBlocking(bool blocking);
	private:
		static bool m_BlockEvents;
	};

}