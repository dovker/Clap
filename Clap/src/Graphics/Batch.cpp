#include "pch.h"
#include "Batch.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Clap
{
    const uint32_t MAX_QUADS = 10000;
    const uint32_t MAX_INDICES = MAX_QUADS * 6;
    const uint32_t MAX_VERTICES = MAX_QUADS * 4;
    const uint32_t MAX_TEXTURES = 16;

    struct CameraBuffer
    {
        glm::mat4 ViewProjection;
    };


    struct RendererData
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
        Ref<Shader> DefaultShader;
        
        uint32_t IndexCount = 0;

        QuadVertex* QuadBuffer = nullptr;
        QuadVertex* QuadPointer = nullptr;

        std::array<Ref<Texture2D>, MAX_TEXTURES> TextureSlots;
        uint32_t CurrentTexIndex = 1;

        Ref<Shader> CurrentShader;
    };

    static RendererData s_Data;

    void Batch::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex));

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraBuffer), 0);
        
        s_Data.QuadVertexBuffer->SetLayout({
            { GraphicsDataType::FLOAT3, "aPos" },
            { GraphicsDataType::FLOAT4, "aColor" },
            { GraphicsDataType::FLOAT2, "aTexCoords" },
            { GraphicsDataType::FLOAT, "aTexIndex" }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        
        s_Data.QuadBuffer = new QuadVertex[MAX_VERTICES];
        
        uint32_t* indices = new uint32_t[MAX_INDICES];
        
        uint32_t offset = 0;
        for (int i = 0; i < MAX_INDICES; i += 6)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }
        
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, MAX_INDICES);
        
        s_Data.QuadVertexArray->SetIndexBuffer(indexBuffer);
        
        delete[] indices;

        
        s_Data.DefaultShader = Shader::Create("../../Game2D/res/Default2D.glsl");
        s_Data.DefaultShader->SetUniformBufferBinding(s_Data.CameraUniformBuffer, "Camera");
        s_Data.CurrentShader = s_Data.DefaultShader;
        

        s_Data.TextureSlots[0] = 0;
        
        
    }
    void Batch::Shutdown()
    {
        delete[] s_Data.QuadBuffer;
    }
    void Batch::SetShader(Ref<Shader> shader) //Only to be called before Batch::Begin
    {
        s_Data.CurrentShader = shader;
        s_Data.CurrentShader->SetUniformBufferBinding(s_Data.CameraUniformBuffer, "Camera");
    }

    void Batch::Begin(const glm::mat4& projection, const glm::mat4& view)
    {
        glm::mat4 ViewProjection = projection * glm::inverse(view);
        s_Data.CameraUniformBuffer->SetData(&ViewProjection, sizeof(ViewProjection));

        BeginBatch();
    }
    
    void Batch::BeginBatch()
    {
        s_Data.IndexCount = 0;
        s_Data.QuadPointer = s_Data.QuadBuffer;
        s_Data.CurrentTexIndex = 1;
        int Samplers[MAX_TEXTURES];
        for (int i = 0; i<MAX_TEXTURES; i++)
        {
            Samplers[i] = i;
        }
        s_Data.CurrentShader->Bind();
        s_Data.CurrentShader->SetIntPtr("uTextures", MAX_TEXTURES, Samplers);
    }
    void Batch::End()
    {

        uint32_t dataSize = (uint32_t)( (uint8_t*)s_Data.QuadPointer - (uint8_t*)s_Data.QuadBuffer );
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadBuffer, dataSize);
        Flush();
    }
    void Batch::Flush()
    {
        for(uint32_t i = 1; i < s_Data.CurrentTexIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        
        s_Data.QuadVertexArray->Bind();
        Graphics::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndexCount);
        s_Data.QuadVertexArray->Unbind();
    }

    void Batch::FlushAndReset()
    {
        End();

        BeginBatch();
    }
    void Batch::Submit(const glm::mat4& transform, const glm::vec4& color)
    {
        if (s_Data.IndexCount >= MAX_INDICES)
        {
            FlushAndReset();
        }

        s_Data.QuadPointer->Position = transform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
        s_Data.QuadPointer->TexIndex = 0.0f;
        s_Data.QuadPointer++;

        s_Data.QuadPointer->Position = transform * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
        s_Data.QuadPointer->TexIndex = 0.0f;
        s_Data.QuadPointer++;
        
        s_Data.QuadPointer->Position = transform * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
        s_Data.QuadPointer->TexIndex = 0.0f;
        s_Data.QuadPointer++;
        
        s_Data.QuadPointer->Position = transform * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
        s_Data.QuadPointer->TexIndex = 0.0f;
        s_Data.QuadPointer++;
        
        s_Data.IndexCount += 6;
    }
    void Batch::Submit(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec4& color, bool scaleByTexture, const glm::vec4& uv)
    {
        if (s_Data.IndexCount >= MAX_INDICES)
        {
            FlushAndReset();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.CurrentTexIndex; i++)
        {
            if(*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if(textureIndex == 0.0f)
        {
            if (s_Data.CurrentTexIndex >= MAX_INDICES)
            {
                FlushAndReset();
            }
            textureIndex = (float)s_Data.CurrentTexIndex;
            s_Data.TextureSlots[s_Data.CurrentTexIndex] = texture;
            s_Data.CurrentTexIndex++;
        }

        glm::mat4 newTransform;
        if(scaleByTexture)
            newTransform = glm::scale(transform, {(float)texture->GetWidth(), (float)texture->GetHeight(), 1.0f});


        s_Data.QuadPointer->Position = newTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.x, uv.w);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;

        s_Data.QuadPointer->Position = newTransform * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.z, uv.w);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
    
        s_Data.QuadPointer->Position = newTransform * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.z, uv.y);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
        
        s_Data.QuadPointer->Position = newTransform * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.x, uv.y);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
        
        s_Data.IndexCount += 6;
    }

    void Batch::Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& uv, const glm::vec4& color)
    {
        if (s_Data.IndexCount >= MAX_INDICES)
        {
            FlushAndReset();
        }


        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.CurrentTexIndex; i++)
        {
            if(*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        if(textureIndex == 0.0f)
        {
            if (s_Data.CurrentTexIndex >= MAX_INDICES)
            {
                FlushAndReset();
            }
            textureIndex = (float)s_Data.CurrentTexIndex;
            s_Data.TextureSlots[s_Data.CurrentTexIndex] = texture;
            s_Data.CurrentTexIndex++;
        }
        
        s_Data.QuadPointer->Position = pos;
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.x, uv.w);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;

        s_Data.QuadPointer->Position = { pos.x + size.x, pos.y, pos.z};
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.z, uv.w);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
        
        s_Data.QuadPointer->Position = { pos.x + size.x, pos.y + size.y, pos.z};
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.z, uv.y);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
        
        s_Data.QuadPointer->Position = { pos.x, pos.y + size.y, pos.z};
        s_Data.QuadPointer->Color = color;
        s_Data.QuadPointer->TexCoords = glm::vec2(uv.x, uv.y);
        s_Data.QuadPointer->TexIndex = textureIndex;
        s_Data.QuadPointer++;
                        
        s_Data.IndexCount += 6;
    }


}
