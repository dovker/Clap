#pragma once

namespace Clap
{
    //WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    struct WindowResizeEvent
    {
        int Width;
        int Height;
    };
}