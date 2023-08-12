#include "pch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>
#include "OpenGL.h"

namespace Clap
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size)
    {
        glCreateBuffers(1, &m_ID);
        glNamedBufferData(m_ID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_ID, offset, size, data);
    }

    void OpenGLUniformBuffer::Bind(uint32_t binding)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
    }
}
