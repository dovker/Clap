#include "pch.h"
#include "Mesh.h"
#include "Graphics/API/API.h"

namespace Clap
{
    void Mesh::m_GenerateVertexArray()
    {
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(float));
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Count);
        m_VertexArray = VertexArray::Create();

        if(m_Tangents)
            vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, "Position"}, {GraphicsDataType::FLOAT2, "TexCoords"}, {GraphicsDataType::FLOAT3, "Normals"}, {GraphicsDataType::FLOAT3, "Tangents"}});
        else
            vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, "Position"}, {GraphicsDataType::FLOAT2, "TexCoords"}, {GraphicsDataType::FLOAT3, "Normals"}});

        m_VertexArray->AddVertexBuffer(vertexBuffer);
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

    Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, bool tangents)
        : m_Vertices(vertices), m_Indices(indices), m_Count(indices.size()), m_Tangents(tangents)
    {
        m_GenerateVertexArray();
    }
    
    Mesh::Mesh(float* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount, bool tangents)
        : m_Count(indexCount), m_Tangents(tangents)
    {
        m_Vertices = std::vector(vertices, vertices + bufferSize); //TODO: FIX THIS
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        m_GenerateVertexArray();
    }

    Mesh::Mesh(const Ref<VertexArray>& vertexArray, bool tangents)
        : m_VertexArray(vertexArray), m_Tangents(tangents)
    {
        m_Count = m_VertexArray->GetIndexBuffer()->GetCount();
    }

    void Mesh::SetTangents(bool tan) { m_Tangents = tan; }
    bool Mesh::GetTangents() const { return m_Tangents; } //Check if mesh contains precalculated tangents.
    
    // void Mesh::CalculateTangents()
    // {
    //     CLAP_ASSERT(!m_Tangents, "Tangent values already generated.");
    //     if(!m_Tangents)
    //     {
    //         std::vector<float> vertices;
    //         m_Tangents = true;

    //         Ref<IndexBuffer> indexBuffer = m_VertexArray->GetIndexBuffer();
    //         Ref<VertexBuffer> vertexBuffer = m_VertexArray->GetVertexBuffers()[0];
    //         m_VertexArray = VertexArray::Create();

    //         vertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(float));
    //         vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, "Position"}, {GraphicsDataType::FLOAT2, "TexCoords"}, {GraphicsDataType::FLOAT3, "Normals"}, {GraphicsDataType::FLOAT4, "Tangents"}});

    //         m_VertexArray->AddVertexBuffer(vertexBuffer);
    //         m_VertexArray->SetIndexBuffer(indexBuffer);
    //     }
    // }

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

    Ref<Mesh> Mesh::Create(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, bool tangents)
    {
        return CreateRef<Mesh>(vertices, indices, tangents);
    }
    Ref<Mesh> Mesh::Create(float* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount, bool tangents)
    {
        return CreateRef<Mesh>(vertices, bufferSize, indices, indexCount, tangents);
    }
    Ref<Mesh> Mesh::Create(const Ref<VertexArray>& vertexArray, bool tangents)
    {
        return CreateRef<Mesh>(vertexArray);
    }
}