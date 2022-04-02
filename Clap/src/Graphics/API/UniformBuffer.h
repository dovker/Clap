#pragma once

#include "Core/Core.h"

namespace Clap
{
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;

        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
        virtual uint32_t GetBinding() = 0;

        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };
}