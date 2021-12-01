#pragma once

#include "Graphics/API/Context.h"

namespace Clap
{
    class SDLOpenGLContext : public GraphicsContext
    {
    public:
        SDLOpenGLContext(Window* window);
        virtual ~SDLOpenGLContext();

        virtual void SetSwapInterval(bool interval) override;
        virtual void SwapBuffers() override;

        static void ForceModernOpenGL();

        virtual void* GetPointer() override { return m_Context; }
    private:
        void* m_Context;
        void* m_Window;
    };
}