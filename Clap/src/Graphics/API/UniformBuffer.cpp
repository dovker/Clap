#include "pch.h"

#include "UniformBuffer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Clap
{
    
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLUniformBuffer>(size, binding);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    
}
