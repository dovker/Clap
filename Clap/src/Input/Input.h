#pragma once
#include "pch.h"

#include "Core/Core.h"

namespace Clap
{
	class  Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePos(); 
		static float GetMouseX();
		static float GetMouseY();

	};
}