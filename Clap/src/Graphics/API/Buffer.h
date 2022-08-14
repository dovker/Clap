#pragma once
#include <pch.h>

#include "Types.h"

#include "Core/Core.h"
#include "Core/Log.h"

#include <glm/glm.hpp>

namespace Clap
{
    static uint32_t GraphicsDataTypeSize(GraphicsDataType type)
    {
        switch (type) {
            case GraphicsDataType::FLOAT:     return 4;
            case GraphicsDataType::FLOAT2:    return 4 * 2;
            case GraphicsDataType::FLOAT3:    return 4 * 3;
            case GraphicsDataType::FLOAT4:    return 4 * 4;
            case GraphicsDataType::MAT3:      return 4 * 3 * 3;
            case GraphicsDataType::MAT4:      return 4 * 4 * 4;
            case GraphicsDataType::INT:       return 4;
            case GraphicsDataType::INT2:      return 4 * 2;
            case GraphicsDataType::INT3:      return 4 * 3;
            case GraphicsDataType::INT4:      return 4 * 4;
            case GraphicsDataType::BOOL:      return 1;
            default: break;
        }
        CLAP_ASSERT(false, "Unknown Graphics data type");
        return 0;
    }

    enum class Attribute
    {
        POSITION,
        NORMAL,
        TANGENT,
        BITANGENT,
        COLOR0,
        COLOR1,
        COLOR2,
        COLOR3,
        INDICES,
        WEIGHT,
        TEXCOORD0,
        TEXCOORD1,
        TEXCOORD2,
        TEXCOORD3,
        TEXCOORD4,
        TEXCOORD5,
        TEXCOORD6,
        TEXCOORD7,
        COUNT,
    };

    struct BufferElement
    {
        BufferElement() = default;
        
        Attribute Name;
        GraphicsDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;
        
        BufferElement(GraphicsDataType type, Attribute name, bool normalized = false)
            : Name(name), Type(type), Size(GraphicsDataTypeSize(type)), Offset(0), Normalized(normalized)
        {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case GraphicsDataType::FLOAT:   return 1;
                case GraphicsDataType::FLOAT2:  return 2;
                case GraphicsDataType::FLOAT3:  return 3;
                case GraphicsDataType::FLOAT4:  return 4;
                case GraphicsDataType::MAT3:    return 3 * 3;
                case GraphicsDataType::MAT4:    return 4 * 4;
                case GraphicsDataType::INT:     return 1;
                case GraphicsDataType::INT2:    return 2;
                case GraphicsDataType::INT3:    return 3;
                case GraphicsDataType::INT4:    return 4;
                case GraphicsDataType::BOOL:    return 1;
                default: break;
            }

            CLAP_ASSERT(false, "Unknown Graphics Data Type!");
            return 0;
        }
    };

    struct BufferLayout
    {
    public:
        BufferLayout(){}
        BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }
        
        uint32_t GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }
        
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        void CalculateOffsetsAndStride();
        
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {}
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        
        virtual void SetData(const void* data, uint32_t size) = 0;
        
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

        static Ref<VertexBuffer> Create(uint32_t size);
    };


    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual uint32_t GetCount() const = 0;
        
        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };
}
