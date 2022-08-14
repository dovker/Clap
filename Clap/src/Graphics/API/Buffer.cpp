#include "pch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Clap
{
    void BufferLayout::CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {   
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLVertexBuffer>(vertices, size);
        #elif CLAP_USE_BGFX
            return nullptr;
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLVertexBuffer>(size);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLIndexBuffer>(indices, count);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
}