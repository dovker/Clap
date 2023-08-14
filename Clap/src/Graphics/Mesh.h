#pragma once

#include "Graphics/API/VertexArray.h"
#include "Core/Core.h"
#include "Math/Math.h"

namespace Clap
{
    #define CLAP_MAX_BONE_INFLUENCE 4

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        uint32_t BoneIDs[CLAP_MAX_BONE_INFLUENCE];
        float Weights[CLAP_MAX_BONE_INFLUENCE];
    };

    struct SimpleVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    class Mesh
    {
    public:
        Mesh(std::vector<SimpleVertex> vertices, const std::vector<uint32_t>& indices);
        Mesh(SimpleVertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount);
        
        Mesh(std::vector<Vertex> vertices, const std::vector<uint32_t>& indices);
        Mesh(Vertex* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount);

        Mesh(float* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount, bool simple = false);

        Mesh(const Ref<VertexArray>& vertexArray, uint32_t count = 0, uint32_t offset = 0, bool simple = false);
        Mesh(const Ref<VertexBuffer>& vertexBuffer, const std::vector<uint32_t>& indices, bool simple = false);
        Mesh(const Ref<VertexBuffer>& vertexBuffer, uint32_t* indices, uint32_t indexCount, bool simple = false);
        
        //Flip Faces, so on
        void FreeData();
        void SetInstanceData(const Ref<VertexBuffer>& vertexBuffer);
        void Draw() const; //Raw draw call
        void DrawInstanced(uint32_t count) const; //Raw draw call

        bool IsCulled() { return m_Culled; }
        AABB GetAABB() { return m_AABB; }

        static Ref<Mesh> Create(std::vector<SimpleVertex> vertices, const std::vector<uint32_t>&);
        static Ref<Mesh> Create(SimpleVertex* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount);

        static Ref<Mesh> Create(std::vector<Vertex> vertices, const std::vector<uint32_t>&);
        static Ref<Mesh> Create(Vertex* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount);
        
        static Ref<Mesh> Create(float* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount, bool simple = false);
   
        static Ref<Mesh> Create(const Ref<VertexArray>& vertexArray, uint32_t count = 0, uint32_t offset = 0, bool simple = false);
        static Ref<Mesh> Create(const Ref<VertexBuffer>& vertexBuffer, const std::vector<uint32_t>& indices, bool simple = false);
        static Ref<Mesh> Create(const Ref<VertexBuffer>& vertexBuffer, uint32_t* indices, uint32_t indexCount, bool simple = false);
        
    private:
        std::vector<float> m_Vertices;
        std::vector<uint32_t> m_Indices;
        uint32_t m_Offset = 0;
        uint32_t m_Count = 0;
        Ref<VertexArray> m_VertexArray;
        AABB m_AABB;

        bool m_Culled = true;

        bool m_Simple = false;
    
    private:
        void m_GenerateVertexArray();
        void m_GenerateAABB();
        void m_GenerateSimpleVertexArray();
        void m_GenerateVertexArrayFromVB(const Ref<VertexBuffer>& vertexBuffer);
        void m_GenerateSimpleVertexArrayFromVB(const Ref<VertexBuffer>& vertexBuffer);
    };
} 