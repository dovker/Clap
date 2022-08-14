#include "pch.h"

#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Clap
{
    
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLFramebuffer>(spec);
        #elif CLAP_USE_BGFX
            return nullptr;
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    
}
