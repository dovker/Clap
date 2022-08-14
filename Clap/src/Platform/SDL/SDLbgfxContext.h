#pragma once

#include "Graphics/API/Context.h"

namespace Clap
{
    class SDLbgfxContext : public GraphicsContext
    {
    public:
        SDLbgfxContext(Window* window);
        virtual ~SDLbgfxContext();

        virtual void SetSwapInterval(bool interval) override;
        virtual void SwapBuffers() override;

        virtual void* GetPointer() override { return nullptr; }
    private:
        Window* m_Window;
    };
}