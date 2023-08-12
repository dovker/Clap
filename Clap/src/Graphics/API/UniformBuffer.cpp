#include "pch.h"

#include "Core/Log.h"

#include "UniformBuffer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Clap
{
    
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLUniformBuffer>(size);
        #elif CLAP_USE_BGFX
            return nullptr;
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    
}
