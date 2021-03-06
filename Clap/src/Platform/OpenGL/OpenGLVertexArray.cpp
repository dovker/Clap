#include "pch.h"

#include "OpenGLVertexArray.h"
#include "Graphics/API/Buffer.h"

#include "glad/glad.h"
#include "OpenGL.h"


namespace Clap
{
    OpenGLVertexArray::OpenGLVertexArray()
    {
         #ifdef CLAP_OPENGL_4_5
            glCreateVertexArrays(1, &m_ID);
        #else
            glGenVertexArrays(1, &m_ID);
        #endif
    }
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }
    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }
    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_ID);
        CheckGPUError();
        
        CLAP_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
        vertexBuffer->Bind();
        CheckGPUError();
        
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            CheckGPUError();
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                GraphicsDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            index++;
            CheckGPUError();
        }
        
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::AttachInstanceBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        glBindVertexArray(m_ID);
        CheckGPUError();
        CLAP_ASSERT(m_VertexBuffers[0], "No Vertex buffer set!");
        CLAP_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        vertexBuffer->Bind();
        
        const auto& layout = vertexBuffer->GetLayout();
        uint32_t index = layout.GetElements().size();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            CheckGPUError();
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                GraphicsDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            CheckGPUError();
            glVertexAttribDivisor(index, 1);
            CheckGPUError();
            index++;
        }
        
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_ID);
        CheckGPUError();
        indexBuffer->Bind();
        CheckGPUError();

        m_IndexBuffer = indexBuffer;
        CheckGPUError();
    }
}
