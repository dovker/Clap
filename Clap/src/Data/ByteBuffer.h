#pragma once

#include "pch.h"
#include "Core/Core.h"

typedef unsigned char byte;

namespace Clap
{
    class ByteBuffer
    {
    public:
        ByteBuffer(void *buffer, size_t size);
        ByteBuffer(std::vector<byte> buffer);
        ByteBuffer(const std::string& filepath);

        std::vector<byte>& GetData() { return m_Data; }
        void SetData(std::vector<byte> buffer) { m_Data = buffer; }

        static Ref<ByteBuffer> Create(void *buffer, size_t size);
        static Ref<ByteBuffer> Create(const std::string& filepath);
        static Ref<ByteBuffer> Create(const std::vector<byte> buffer);
    private:
        std::vector<byte> m_Data;
    };
}