#pragma once
#include <pch.h>

#include "Graphics/API/Buffer.h"

namespace Clap
{
    class bgfxVertexBuffer : public VertexBuffer
    {
    public:
        bgfxVertexBuffer(float* vertices, uint32_t size);

        bgfxVertexBuffer(uint32_t size);
        
        virtual ~bgfxVertexBuffer();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() const override { return m_Layout; };
        
        virtual void SetData(const void* data, uint32_t size) override;
    
    private:
        uint32_t m_ID;
        BufferLayout m_Layout;
    };
    class bgfxIndexBuffer : public IndexBuffer
    {
    public:
        bgfxIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~bgfxIndexBuffer();
        
        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual uint32_t GetCount() const override { return m_Count; };
        
    private:
        uint32_t m_ID, m_Count;
    };
}