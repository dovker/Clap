#pragma once

namespace Clap
{
    //MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

    struct MouseButtonEvent
    {
        int MouseCode;
    };

    struct MouseEvent
    {
        float XAxis;
        float YAxis;
    };
}