#pragma once
#include <pch.h>

#include "Graphics/API/UniformBuffer.h"

namespace Clap
{
    class bgfxUniformBuffer : public UniformBuffer
    {
    public:
        bgfxUniformBuffer(uint32_t size, uint32_t binding);
        ~bgfxUniformBuffer();

        virtual void SetData(const void* data, uint32_t size, uint32_t offset) override;
        virtual uint32_t GetBinding() override { return m_Binding; };
        
    private:
        uint32_t m_ID, m_Binding;
    };
}