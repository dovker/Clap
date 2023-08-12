#include "pch.h"
#include "Mesh.h"
#include "Graphics/API/API.h"

namespace Clap
{
    void Mesh::m_GenerateVertexArrayFromVB(const Ref<VertexBuffer>& vertexBuffer)
    {
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Count);
        m_VertexArray = VertexArray::Create();

        vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, Attribute::POSITION}, {GraphicsDataType::FLOAT2, Attribute::TEXCOORD0}, 
                                  {GraphicsDataType::FLOAT3, Attribute::NORMAL}, {GraphicsDataType::FLOAT3, Attribute::TANGENT},
                                  {GraphicsDataType::FLOAT3, Attribute::BITANGENT}, {GraphicsDataType::INT4, Attribute::INDICES},
                                  {GraphicsDataType::INT4, Attribute::WEIGHT}});

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    void Mesh::m_GenerateSimpleVertexArrayFromVB(const Ref<VertexBuffer>& vertexBuffer)
    {
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Count);
        m_VertexArray = VertexArray::Create();

        vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, Attribute::POSITION}, {GraphicsDataType::FLOAT2, Attribute::TEXCOORD0}, 
                                  {GraphicsDataType::FLOAT3, Attribute::NORMAL}});

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    void Mesh::m_GenerateVertexArray()
    {
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(float));
        m_GenerateVertexArrayFromVB(vertexBuffer);
    }

    void Mesh::m_GenerateSimpleVertexArray()
    {
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(float));
        m_GenerateSimpleVertexArrayFromVB(vertexBuffer);
    }

    Mesh::Mesh(std::vector<Vertex> vertices, const std::vector<uint32_t>& indices)
        : m_Indices(indices), m_Count(indices.size())
    {
        m_Vertices = std::vector<float>((float*)std::data(vertices), (float*)std::data(vertices) + vertices.size() * sizeof(float));
        m_GenerateVertexArray();
    }
    
    Mesh::Mesh(Vertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount)
        : m_Count(indexCount)
    {
        std::copy((float*)vertices, (float*)vertices + bufferSize, std::back_inserter(m_Vertices));
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        m_GenerateVertexArray();
    }
    
    Mesh::Mesh(float* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount, bool simple)
        : m_Count(indexCount), m_Simple(simple)
    {
        std::copy(vertices, vertices + bufferSize, std::back_inserter(m_Vertices));
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        if (!simple)
            m_GenerateVertexArray();
        else
            m_GenerateSimpleVertexArray();
    }

    Mesh::Mesh(std::vector<SimpleVertex> vertices, const std::vector<uint32_t>& indices)
        : m_Indices(indices), m_Count(indices.size()), m_Simple(true)
    {
        m_Vertices = std::vector<float>((float*)std::data(vertices), (float*)std::data(vertices) + vertices.size() * sizeof(float));
        m_GenerateSimpleVertexArray();
    }
    
    Mesh::Mesh(SimpleVertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount)
        : m_Count(indexCount), m_Simple(true)
    {
        std::copy((float*)vertices, (float*)vertices + bufferSize, std::back_inserter(m_Vertices));
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        m_GenerateSimpleVertexArray();
    }

    Mesh::Mesh(const Ref<VertexArray>& vertexArray, bool simple)
        : m_VertexArray(vertexArray), m_Simple(simple)
    {
        m_Count = m_VertexArray->GetIndexBuffer()->GetCount();
    }
   
    Mesh::Mesh(const Ref<VertexBuffer>& vertexBuffer, const std::vector<uint32_t>& indices, bool simple)
        : m_Simple(simple), m_Indices(indices), m_Count(indices.size())
    {
        if (!m_Simple)
            m_GenerateVertexArrayFromVB(vertexBuffer);
        else
            m_GenerateSimpleVertexArrayFromVB(vertexBuffer);
    }

    Mesh::Mesh(const Ref<VertexBuffer>& vertexBuffer, uint32_t* indices, uint32_t indexCount, bool simple)
        : m_Simple(simple)
    {
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        if (!m_Simple)
            m_GenerateVertexArrayFromVB(vertexBuffer);
        else
            m_GenerateSimpleVertexArrayFromVB(vertexBuffer);
    }

    void Mesh::FreeData()
    {
        m_Vertices.clear();
        m_Indices.clear();
    }
    void Mesh::Draw() const
    {
        Graphics::DrawIndexed(m_VertexArray);
    }
    void Mesh::SetInstanceData(const Ref<VertexBuffer>& vertexBuffer)
    {
        m_VertexArray->AttachInstanceBuffer(vertexBuffer);
    }
    void Mesh::DrawInstanced(uint32_t count) const
    {
        Graphics::DrawIndexedInstanced(m_VertexArray, count);
    }

    Ref<Mesh> Mesh::Create(std::vector<Vertex> vertices, const std::vector<uint32_t>& indices)
    {
        return CreateRef<Mesh>(vertices, indices);
    }

    Ref<Mesh> Mesh::Create(Vertex* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount)
    {
        return CreateRef<Mesh>(vertices, bufferSize, indices, indexCount);
    }


    Ref<Mesh> Mesh::Create(std::vector<SimpleVertex> vertices, const std::vector<uint32_t>& indices)
    {
        return CreateRef<Mesh>(vertices, indices);
    }

    Ref<Mesh> Mesh::Create(SimpleVertex* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount)
    {
        return CreateRef<Mesh>(vertices, bufferSize, indices, indexCount);
    }


    Ref<Mesh> Mesh::Create(float* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount, bool simple)
    {
        return CreateRef<Mesh>(vertices, bufferSize, indices, indexCount, simple);
    }


    Ref<Mesh> Mesh::Create(const Ref<VertexArray>& vertexArray, bool simple)
    {
        return CreateRef<Mesh>(vertexArray, simple);
    }

    Ref<Mesh> Mesh::Create(const Ref<VertexBuffer>& vertexBuffer, const std::vector<uint32_t>& indices, bool simple)
    {
        return CreateRef<Mesh>(vertexBuffer, indices, simple);
    }

    Ref<Mesh> Mesh::Create(const Ref<VertexBuffer>& vertexBuffer, uint32_t* indices, uint32_t indexCount, bool simple)
    {
        return CreateRef<Mesh>(vertexBuffer, indices, indexCount, simple);
    }

}