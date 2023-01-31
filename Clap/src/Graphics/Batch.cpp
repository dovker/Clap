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

        glm::vec4 QuadPositions[4] = {
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
        };
    };

    static RendererData s_Data;

    void Batch::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex));

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraBuffer), 0);
        
        s_Data.QuadVertexBuffer->SetLayout({
            { GraphicsDataType::FLOAT3, Attribute::POSITION },
            { GraphicsDataType::FLOAT4, Attribute::COLOR0 },
            { GraphicsDataType::FLOAT2, Attribute::TEXCOORD0 },
            { GraphicsDataType::FLOAT4, Attribute::TEXCOORD1 }
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

        
        s_Data.DefaultShader = Shader::Create("C:/dev/Clap/Clap/res/Default2D.glsl");
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
        
        s_Data.CurrentShader->Bind();
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
        
        Graphics::DrawIndexed(s_Data.QuadVertexArray, s_Data.IndexCount);
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

        for(int i = 0; i < 4; i++)
        {
            s_Data.QuadPointer->Position = transform * s_Data.QuadPositions[i];
            s_Data.QuadPointer->Color = color;
            s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
            s_Data.QuadPointer->Data.x = 0.0f; //TexIndex
            s_Data.QuadPointer++;
        }
        s_Data.IndexCount += 6;
    }

    void Batch::Submit(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, float radius, float outlineRadius)
    {
        if (s_Data.IndexCount >= MAX_INDICES)
        {
            FlushAndReset();
        }

        glm::vec3 positions[4] = 
        {
            pos,
            { pos.x + size.x, pos.y, pos.z},
            { pos.x + size.x, pos.y + size.y, pos.z},
            { pos.x, pos.y + size.y, pos.z}
        };
        
        for(int i = 0; i < 4; i++)
        {
            s_Data.QuadPointer->Position = positions[i];
            s_Data.QuadPointer->Color = color;
            s_Data.QuadPointer->TexCoords = glm::vec2(0.0f);
            s_Data.QuadPointer->Data.x = 0.0f; // Texture Index
            s_Data.QuadPointer->Data.z = radius; // Radius
            s_Data.QuadPointer->Data.w = outlineRadius; // Outline Radius
            s_Data.QuadPointer++;
        }
                        
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
            newTransform = glm::scale(transform, {(float)texture->GetWidth() * glm::abs(uv.x-uv.z), (float)texture->GetHeight() * glm::abs(uv.y-uv.w), 1.0f});

        glm::vec2 uvs[4] = 
        {
            glm::vec2(uv.x, uv.w),
            glm::vec2(uv.z, uv.w),
            glm::vec2(uv.z, uv.y),
            glm::vec2(uv.x, uv.y)
        };

        for(int i = 0; i < 4; i++)
        {
            s_Data.QuadPointer->Position = newTransform * s_Data.QuadPositions[i];
            s_Data.QuadPointer->Color = color;
            s_Data.QuadPointer->TexCoords = uvs[i];
            s_Data.QuadPointer->Data.x = textureIndex; //TexIndex
            s_Data.QuadPointer++;
        }

        s_Data.IndexCount += 6;
    }

    void Batch::Submit(const glm::mat4& transform, Ref<Texture2D>& texture, const glm::vec2& from, const glm::vec2& to, const glm::vec4& color, bool scaleByTexture)
    {
        Submit(transform, texture, color, scaleByTexture, {from.x / texture->GetWidth(), 1.0f - to.y / texture->GetHeight(), to.x / texture->GetWidth(), 1.0f - from.y / texture->GetHeight()});
    }

    void Batch::Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec2& from, const glm::vec2& to, const glm::vec4& color)
    {
        Submit(pos, size, texture, {from.x / texture->GetWidth(), 1.0f - to.y / texture->GetHeight(), to.x / texture->GetWidth(), 1.0f - from.y / texture->GetHeight()}, color);
    }
    
    void Batch::Submit(const glm::mat4& transform, Ref<Texture2D>& texture, glm::vec3 positions[4], glm::vec2 uv[4], const glm::vec4& color)
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


        for(int i = 0; i < 4; i++)
        {
            s_Data.QuadPointer->Position = transform * glm::vec4(positions[i], 1.0f);
            s_Data.QuadPointer->Color = color;
            s_Data.QuadPointer->TexCoords = uv[i];
            s_Data.QuadPointer->Data.x = textureIndex; //TexIndex
            s_Data.QuadPointer++;
        }

        s_Data.IndexCount += 6;
    }

    void Batch::SubmitTile(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec2& tileSize, uint32_t tile, const glm::vec4& color)
    {
        uint32_t tW = texture->GetWidth();

        float sY = ((uint32_t)(tile * tileSize.y) / tW);
        float sX = ((uint32_t)(tile * tileSize.x) % tW) / tileSize.x;

        Submit(pos, size, texture, {sX * tileSize.x, sY * tileSize.y}, {(sX + 1) * tileSize.x, (sY + 1) * tileSize.y}, color);
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

        glm::vec2 uvs[4] = 
        {
            glm::vec2(uv.x, uv.w),
            glm::vec2(uv.z, uv.w),
            glm::vec2(uv.z, uv.y),
            glm::vec2(uv.x, uv.y)
        };

        glm::vec3 positions[4] = 
        {
            pos,
            { pos.x + size.x, pos.y, pos.z},
            { pos.x + size.x, pos.y + size.y, pos.z},
            { pos.x, pos.y + size.y, pos.z}
        };
        
        for(int i = 0; i < 4; i++)
        {
            s_Data.QuadPointer->Position = positions[i];
            s_Data.QuadPointer->Color = color;
            s_Data.QuadPointer->TexCoords = uvs[i];
            s_Data.QuadPointer->Data.x = textureIndex; //TexIndex
            s_Data.QuadPointer++;
        }
                        
        s_Data.IndexCount += 6;
    }

    // void Submit(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color, float radius = 0.0f, float outlineRadius = 0.0f)
    // {

    // }

}
