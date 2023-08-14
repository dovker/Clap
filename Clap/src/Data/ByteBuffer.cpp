#include "pch.h"

#include "Core/Core.h"
#include "Core/Log.h"
#include "ByteBuffer.h"

namespace Clap
{
    ByteBuffer::ByteBuffer(void *buffer, size_t size)
    {
        m_Data = std::vector<byte>((byte*)buffer, (byte*)buffer + size);
    }
    ByteBuffer::ByteBuffer(size_t size)
    {
        m_Data.resize(size);
    }

    ByteBuffer::ByteBuffer(std::vector<byte> buffer)
        : m_Data(buffer)
    {}

    ByteBuffer::ByteBuffer(const std::string& filepath)
    {
        std::ifstream ifs(filepath, std::ios::in | std::ios::binary);

        ifs.unsetf(std::ios::skipws);

        std::streampos fileSize;

        ifs.seekg(0, std::ios::end);
        fileSize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        CLAP_ASSERT(!ifs.bad() || fileSize < 0, "Error reading file to a byte buffer");

        // reserve capacity
        m_Data.clear();
        m_Data.reserve(fileSize);

        // read the data:
        m_Data.insert(m_Data.begin(),
                std::istream_iterator<byte>(ifs),
                std::istream_iterator<byte>());
    }

    Ref<ByteBuffer> ByteBuffer::Create(void *buffer, size_t size)
    {
        return CreateRef<ByteBuffer>(buffer, size);
    }
    Ref<ByteBuffer> ByteBuffer::Create(size_t size)
    {
        return CreateRef<ByteBuffer>(size);
    }

    Ref<ByteBuffer> ByteBuffer::Create(const std::string& filepath)
    {
        return CreateRef<ByteBuffer>(filepath);
    }
    
    Ref<ByteBuffer> ByteBuffer::Create(const std::vector<byte> buffer)
    {
        return CreateRef<ByteBuffer>(buffer);
    }
}