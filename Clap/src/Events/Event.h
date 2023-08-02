#pragma once
#include "pch.h"

#include "Core/Core.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"


namespace Clap
{
    enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		GamepadButtonPressed, GamepadButtonReleased, GamepadAxisMoved
	};

    struct Event
    {
        EventType Type;
        bool Immediate = false;
		bool Handled = false;
        union EventData
        {
            Clap::WindowResizeEvent WindowResizeEvent;
            Clap::MouseEvent MouseEvent;
            Clap::MouseButtonEvent MouseButtonEvent;
            Clap::KeyEvent KeyEvent;
			Clap::KeyTypedEvent KeyTypedEvent;
        } Data;
    };
}