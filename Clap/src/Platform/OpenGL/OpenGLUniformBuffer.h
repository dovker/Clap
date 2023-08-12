#pragma once
#include <pch.h>

#include "Graphics/API/UniformBuffer.h"

namespace Clap
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(uint32_t size);
        ~OpenGLUniformBuffer();

        virtual void SetData(const void* data, uint32_t size, uint32_t offset) override;        
        virtual void Bind(uint32_t binding) override;        
    private:
        uint32_t m_ID = 0;
    };
}