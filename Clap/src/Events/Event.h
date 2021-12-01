#pragma once
#include "pch.h"

#include "Core/Core.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

#define BIT(x) (1 << x)

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

    enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryGamepad = BIT(5),
		EventCategoryGamepadButton = BIT(6),
		EventCategoryGamepadAxis = BIT(7)
	};

    struct Event
    {
        EventType Type;
        bool Immediate = false;
        union EventData
        {
            WindowResizeEvent WindowResizeEvent;
            MouseEvent MouseEvent;
            MouseButtonEvent MouseButtonEvent;
            KeyEvent KeyEvent;
        } Data;
    };
}