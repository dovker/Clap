#pragma once

#include "Graphics/API/VertexArray.h"
#include "Core/Core.h"

namespace Clap
{
    class Mesh
    {
    public:
        Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, bool tangents = false);
        Mesh(float* vertices, size_t bufferSize, uint32_t* indices, uint32_t indexCount, bool tangents = false);
        Mesh(const Ref<VertexArray>& vertexArray, bool tangents = false);

        void SetTangents(bool tan);
        bool GetTangents() const; //Check if mesh contains precalculated tangents.
        
        
        //void CalculateTangents();
        //Flip Faces, so on
        void FreeData();
        void Draw() const; //Raw draw call

        static Ref<Mesh> Create(const std::vector<float>& vertices, const std::vector<uint32_t>&, bool tangents = false);
        static Ref<Mesh> Create(float* vertices, uint32_t bufferSize, uint32_t* indices, uint32_t indexCount, bool tangents = false);
        static Ref<Mesh> Create(const Ref<VertexArray>& vertexArray, bool tangents = false);
    private:
        std::vector<float> m_Vertices;
        std::vector<uint32_t> m_Indices;
        uint32_t m_Count;
        bool m_Tangents;
        Ref<VertexArray> m_VertexArray;
    
    private:
        void m_GenerateVertexArray();
    };
} 