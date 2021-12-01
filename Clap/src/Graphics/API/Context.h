#pragma once

#include "Core/Core.h"

namespace Clap
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void SetSwapInterval(bool interval) = 0;
        virtual void SwapBuffers() = 0;

        virtual void* GetPointer() = 0;
    };
}