#include "pch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>
#include "OpenGL.h"

namespace Clap
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
        : m_Binding(binding)
    {
        #ifdef CLAP_OPENGL_4_5
            glCreateBuffers(1, &m_ID);
            glNamedBufferData(m_ID, size, nullptr, GL_DYNAMIC_DRAW);
        #else
            glGenBuffers(1, &m_ID);
            CheckGPUError();
            glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
            CheckGPUError();
            glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
            CheckGPUError();
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            CheckGPUError();

            glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
        #endif
        CheckGPUError();
    }
    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }
    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        #ifdef CLAP_OPENGL_4_5
            glNamedBufferSubData(m_ID, offset, size, data);
        #else
            CheckGPUError();
            glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
            CheckGPUError();
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
            CheckGPUError();
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            CheckGPUError();
        #endif
    }
}
