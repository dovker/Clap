#pragma once

#include "Clap.h"

namespace Game2D
{
    class PostProcess //ADD INITIALIZER LIST FOR BINDING ALL COLOR CHANNELS
    {
    public:
        PostProcess(Ref<Framebuffer> framebuffer, Ref<Shader> shader, Ref<UniformBuffer> uniformBuffer, bool useDepth = false)
            : m_Framebuffer(framebuffer), m_Shader(shader), m_UniformBuffer(uniformBuffer), m_UseDepth(useDepth)
        {
            m_QuadVA = VertexArray::Create();
            float vertices[4 * 5] = {
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f 
            };

            Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
            BufferLayout layout = {
                { GraphicsDataType::FLOAT3, "a_Position" },
                { GraphicsDataType::FLOAT2, "a_TexCoords" }
            };
            vertexBuffer->SetLayout(layout);
            m_QuadVA->AddVertexBuffer(vertexBuffer);

            uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
            Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
            m_QuadVA->SetIndexBuffer(indexBuffer);
            m_QuadVA->Unbind();

            m_Shader->SetUniformBufferBinding(m_UniformBuffer, "PostProcess");
        }

        void SetData(const void* data, uint32_t size)
        {
            m_UniformBuffer->SetData(data, size);
        }

        void Begin()
        {
            m_Framebuffer->Bind();
            Graphics::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});
            Graphics::Clear();
        }
        void End()
        {
            //For each effect pass, draw it onto the surface;
            m_Framebuffer->Unbind();
            m_Shader->Bind();
            auto specification = m_Framebuffer->GetSpecification();
            int i = 0;
            for(TextureSpecification spec : specification.Attachments.Attachments)
            {
                if(spec.Format != TextureFormat::DEPTH24STENCIL8)
                {
                    m_Shader->SetInt("gColor_" + i, i);
                    Graphics::BindTexture(m_Framebuffer->GetColorAttachmentRendererID(i), i);
                    i++;
                }
            }
            if(m_UseDepth)
            {
                m_Shader->SetInt("gDepth", i);
                Graphics::BindTexture(m_Framebuffer->GetDepthAttachmentRendererID(), i);
            }

            m_QuadVA->Bind();
            Graphics::SetClearColor({0.3f, 0.4f, 0.45f, 1.0f});
            Graphics::Clear();

            Graphics::DrawIndexed(m_QuadVA);
            m_QuadVA->Unbind();

            m_Shader->Unbind();
        }

        static Ref<PostProcess> Create(Ref<Framebuffer> framebuffer, Ref<Shader> shader, Ref<UniformBuffer> uniformBuffer)
        {
            return CreateRef<PostProcess>(framebuffer, shader, uniformBuffer);
        }

    private:
        Ref<Shader> m_Shader;
        Ref<UniformBuffer> m_UniformBuffer;
        Ref<Framebuffer> m_Framebuffer;
        Ref<VertexArray> m_QuadVA;

        bool m_UseDepth;
    };
}