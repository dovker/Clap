#pragma once
#include <pch.h>

#include "Graphics/API/UniformBuffer.h"

namespace Clap
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        ~OpenGLUniformBuffer();

        virtual void SetData(const void* data, uint32_t size, uint32_t offset) override;
        virtual uint32_t GetBinding() { return m_Binding; };
        
    private:
        uint32_t m_ID, m_Binding;
    };
}