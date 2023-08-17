#include "pch.h"
#include "Mesh.h"
#include "Graphics/API/API.h"

namespace Clap
{
    void Mesh::m_GenerateVertexArrayFromVB(const Ref<VertexBuffer>& vertexBuffer)
    {
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Count);
        m_VertexArray = VertexArray::Create();

        vertexBuffer->SetLayout({ {GraphicsDataType::FLOAT3, Attribute::POSITION}, {GraphicsDataType::FLOAT3, Attribute::NORMAL}, 
                                  {GraphicsDataType::FLOAT2, Attribute::TEXCOORD0}, {GraphicsDataType::FLOAT3, Attribute::TANGENT},
                                  //{GraphicsDataType::INT4, Attribute::INDICES}, {GraphicsDataType::INT4, Attribute::WEIGHT}
                                  });

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

    void Mesh::m_GenerateAABB()
    {
        size_t vertexSize = m_Simple ? sizeof(SimpleVertex) : sizeof(Vertex);
        glm::vec3 minPos;
        glm::vec3 maxPos;

        minPos = maxPos = glm::vec3(m_Vertices[0], m_Vertices[1], m_Vertices[2]);

        for(size_t i = 0; i < m_Vertices.size(); i += vertexSize)
        {
            minPos.x = glm::min(minPos.x, m_Vertices[i]);
            minPos.y = glm::min(minPos.y, m_Vertices[i+1]);
            minPos.z = glm::min(minPos.z, m_Vertices[i+2]);

            maxPos.x = glm::max(maxPos.x, m_Vertices[i]);
            maxPos.y = glm::max(maxPos.y, m_Vertices[i+1]);
            maxPos.z = glm::max(maxPos.z, m_Vertices[i+2]);
        }

        m_AABB = { minPos, maxPos };
    }

    Mesh::Mesh(std::vector<Vertex> vertices, const std::vector<uint32_t>& indices)
        : m_Indices(indices), m_Count(indices.size())
    {
        m_Vertices = std::vector<float>((float*)std::data(vertices), (float*)std::data(vertices) + vertices.size() * sizeof(Vertex) / sizeof(float));
        m_GenerateVertexArray();
        m_GenerateAABB();
    }
    
    Mesh::Mesh(Vertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount)
        : m_Count(indexCount)
    {
        std::copy((float*)vertices, (float*)vertices + bufferSize, std::back_inserter(m_Vertices));
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        m_GenerateVertexArray();
        m_GenerateAABB();
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
        m_GenerateAABB();
    }

    Mesh::Mesh(std::vector<SimpleVertex> vertices, const std::vector<uint32_t>& indices)
        : m_Indices(indices), m_Count(indices.size()), m_Simple(true)
    {
        m_Vertices = std::vector<float>((float*)std::data(vertices), (float*)std::data(vertices) + vertices.size() * sizeof(SimpleVertex)  / sizeof(float));
        m_GenerateSimpleVertexArray();
        m_GenerateAABB();
    }
    
    Mesh::Mesh(SimpleVertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount)
        : m_Count(indexCount), m_Simple(true)
    {
        std::copy((float*)vertices, (float*)vertices + bufferSize, std::back_inserter(m_Vertices));
        m_Indices = std::vector(indices, indices + indexCount * sizeof(uint32_t));
        m_GenerateSimpleVertexArray();
        m_GenerateAABB();
    }

    Mesh::Mesh(const Ref<VertexArray>& vertexArray, uint32_t count, uint32_t offset, bool simple)
        : m_VertexArray(vertexArray), m_Simple(simple), m_Count(count), m_Culled(false) //TODO: Implement a better way of setting culling
    {
        if (m_Count == 0 )
            m_Count = m_VertexArray->GetIndexBuffer()->GetCount();
    }
   
    Mesh::Mesh(const Ref<VertexBuffer>& vertexBuffer, const std::vector<uint32_t>& indices, bool simple)
        : m_Simple(simple), m_Indices(indices), m_Count(indices.size()), m_Culled(false)
    {
        if (!m_Simple)
            m_GenerateVertexArrayFromVB(vertexBuffer);
        else
            m_GenerateSimpleVertexArrayFromVB(vertexBuffer);
    }

    Mesh::Mesh(const Ref<VertexBuffer>& vertexBuffer, uint32_t* indices, uint32_t indexCount, bool simple)
        : m_Simple(simple), m_Culled(false)
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

    void CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        std::vector<glm::vec3> tan1(vertices.size(), glm::vec3(0.0f));
        std::vector<glm::vec3> tan2(vertices.size(), glm::vec3(0.0f));

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            uint32_t i1 = indices[i];
            uint32_t i2 = indices[i + 1];
            uint32_t i3 = indices[i + 2];

            const glm::vec3& v1 = vertices[i1].Position;
            const glm::vec3& v2 = vertices[i2].Position;
            const glm::vec3& v3 = vertices[i3].Position;

            const glm::vec2& w1 = vertices[i1].TexCoord;
            const glm::vec2& w2 = vertices[i2].TexCoord;
            const glm::vec2& w3 = vertices[i3].TexCoord;

            glm::vec3 deltaPos1 = v2 - v1;
            glm::vec3 deltaPos2 = v3 - v1;

            glm::vec2 deltaUV1 = w2 - w1;
            glm::vec2 deltaUV2 = w3 - w1;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            glm::vec3 sdir = glm::vec3((deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x) * r,
                                    (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y) * r,
                                    (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z) * r);
            glm::vec3 tdir = glm::vec3((deltaUV1.x * deltaPos2.x - deltaUV2.x * deltaPos1.x) * r,
                                    (deltaUV1.x * deltaPos2.y - deltaUV2.x * deltaPos1.y) * r,
                                    (deltaUV1.x * deltaPos2.z - deltaUV2.x * deltaPos1.z) * r);

            tan1[i1] += sdir;
            tan1[i2] += sdir;
            tan1[i3] += sdir;

            tan2[i1] += tdir;
            tan2[i2] += tdir;
            tan2[i3] += tdir;
        }

        for (size_t a = 0; a < vertices.size(); ++a)
        {
            const glm::vec3& n = vertices[a].Normal;
            const glm::vec3& t = tan1[a];

            glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));
            vertices[a].Tangent = glm::vec4(tangent, (glm::dot(glm::cross(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f);
        }
    }

    Ref<Mesh> Mesh::Sphere(uint32_t subdivisions, float radius)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Generate vertices
        for (uint32_t face = 0; face < 6; ++face) {
            for (uint32_t lat = 0; lat <= subdivisions; ++lat) {
                for (uint32_t lon = 0; lon <= subdivisions; ++lon) {
                    float u = static_cast<float>(lon) / subdivisions;
                    float v = static_cast<float>(lat) / subdivisions;

                    glm::vec3 position, normal;

                    // Generate positions and normals based on face
                    switch (face) {
                        case 0: // +X face
                            position = glm::normalize(glm::vec3(1.0f, v * 2.0f - 1.0f, u * 2.0f - 1.0f));
                            normal = glm::vec3(1.0f, 0.0f, 0.0f);
                            break;
                        case 1: // -X face
                            position = glm::normalize(glm::vec3(-1.0f, v * 2.0f - 1.0f, -(u * 2.0f - 1.0f)));
                            normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                            break;
                        case 2: // +Y face
                            position = glm::normalize(glm::vec3(u * 2.0f - 1.0f, 1.0f, v * 2.0f - 1.0f));
                            normal = glm::vec3(0.0f, 1.0f, 0.0f);
                            break;
                        case 3: // -Y face
                            position = glm::normalize(glm::vec3(u * 2.0f - 1.0f, -1.0f, -(v * 2.0f - 1.0f)));
                            normal = glm::vec3(0.0f, -1.0f, 0.0f);
                            break;
                        case 4: // +Z face
                            position = glm::normalize(glm::vec3(u * 2.0f - 1.0f, v * 2.0f - 1.0f, 1.0f));
                            normal = glm::vec3(0.0f, 0.0f, 1.0f);
                            break;
                        case 5: // -Z face
                            position = glm::normalize(glm::vec3(u * 2.0f - 1.0f, v * 2.0f - 1.0f, -1.0f));
                            normal = glm::vec3(0.0f, 0.0f, -1.0f);
                            break;
                    }

                    glm::vec3 spherifiedPosition = glm::normalize(position) * radius;
                    glm::vec3 spherifiedNormal = glm::normalize(spherifiedPosition);

                    // Calculate texture coordinates
                    glm::vec2 texCoord(u, v);

                    vertices.push_back({
                        spherifiedPosition,
                        spherifiedNormal,
                        texCoord,
                        glm::vec3(0.0f),  // Tangent (will be computed later)
                    });
                }
            }
        }

        // Generate indices
        for (uint32_t face = 0; face < 6; ++face) {
            bool flip = (face == 4); // Check if it's the front face

            for (uint32_t lat = 0; lat < subdivisions; ++lat) {
                for (uint32_t lon = 0; lon < subdivisions; ++lon) {
                    uint32_t current = face * (subdivisions + 1) * (subdivisions + 1) + lat * (subdivisions + 1) + lon;
                    uint32_t next = current + (subdivisions + 1);

                    if (flip) {
                        indices.push_back(current);
                        indices.push_back(next + 1);
                        indices.push_back(next);
                        indices.push_back(current);
                        indices.push_back(current + 1);
                        indices.push_back(next + 1);
                    } else {
                        indices.push_back(current);
                        indices.push_back(next);
                        indices.push_back(next + 1);
                        indices.push_back(current);
                        indices.push_back(next + 1);
                        indices.push_back(current + 1);
                    }
                }
            }
        }

        // Calculate tangents and bitangents
        CalculateTangents(vertices, indices);

        return Mesh::Create(vertices, indices);
    }

    Ref<Mesh> Mesh::Cube(uint32_t subdivisions, float size)
    {
        CLAP_ASSERT(false, "Cube Generation Unimplemented");
        return nullptr;
    }

    Ref<Mesh> Mesh::Plane(float width, float height, uint32_t segmentsX, uint32_t segmentsY)
    {
        std::vector<Vertex> vertices;        
        std::vector<uint32_t> indices;

        float deltaX = width / static_cast<float>(segmentsX);
        float deltaY = height / static_cast<float>(segmentsY);
        
        for (uint32_t y = 0; y <= segmentsY; ++y) {
            for (uint32_t x = 0; x <= segmentsX; ++x) {
                Vertex vertex;
                vertex.Position = glm::vec3(x * deltaX - width * 0.5f, 0.0f, y * deltaY - height * 0.5f);
                vertex.TexCoord = glm::vec2(static_cast<float>(x) / segmentsX, static_cast<float>(y) / segmentsY);
                vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
                vertices.push_back(vertex);
            }
        }
        
        for (uint32_t y = 0; y < segmentsY; ++y) {
            for (uint32_t x = 0; x < segmentsX; ++x) {
                uint32_t i0 = y * (segmentsX + 1) + x;
                uint32_t i1 = i0 + 1;
                uint32_t i2 = (y + 1) * (segmentsX + 1) + x;
                uint32_t i3 = i2 + 1;
                
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);
                
                indices.push_back(i2);
                indices.push_back(i3);
                indices.push_back(i1);
            }
        }

        return Mesh::Create(vertices, indices);
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


    Ref<Mesh> Mesh::Create(const Ref<VertexArray>& vertexArray, uint32_t count, uint32_t offset, bool simple)
    {
        return CreateRef<Mesh>(vertexArray, count, offset, simple);
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