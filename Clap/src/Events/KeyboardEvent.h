#pragma once

namespace Clap
{
    struct KeyEvent
    {
        int KeyCode;
        int RepeatCount;
    };
    struct KeyTypedEvent
    {
        char Key;
    };
}