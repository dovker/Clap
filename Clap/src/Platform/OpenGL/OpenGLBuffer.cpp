#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>
#include "OpenGL.h"

namespace Clap
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        #ifdef CLAP_OPENGL_4_5
            glCreateBuffers(1, &m_ID);
        #else
            glGenBuffers(1, &m_ID);
        #endif

        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        #ifdef CLAP_DEBUG
        CheckGPUError();
        #endif
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        #ifdef CLAP_OPENGL_4_5
            glCreateBuffers(1, &m_ID);
        #else
            glGenBuffers(1, &m_ID);
        #endif

        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

        #ifdef CLAP_DEBUG
        CheckGPUError();
        #endif
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }
    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }
    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

        #ifdef CLAP_DEBUG
        CheckGPUError();
        #endif
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        #ifdef CLAP_OPENGL_4_5
            glCreateBuffers(1, &m_ID);
        #else
            glGenBuffers(1, &m_ID);
        #endif

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        #ifdef CLAP_DEBUG
        CheckGPUError();
        #endif
    }
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }
    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }
    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
