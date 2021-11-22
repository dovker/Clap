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
    
            glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
            glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        #endif

        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
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
            glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        #endif
    }
}
