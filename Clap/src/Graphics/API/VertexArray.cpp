#include "pch.h"

#include "VertexArray.h"
#include "Core/Core.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Clap
{
    Ref<VertexArray> VertexArray::Create()
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLVertexArray>();
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
}
