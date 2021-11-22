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
        
        CLAP_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
        
        glBindVertexArray(m_ID);
        vertexBuffer->Bind();
        
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                GraphicsDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            index++;
        }
        
        m_VertexBuffers.push_back(vertexBuffer);
    }
    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_ID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}
