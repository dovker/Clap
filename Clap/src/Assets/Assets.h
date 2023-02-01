#pragma once

namespace Clap
{
    class Asset
    {
    public:
        static void Load(const std::string& path);
        static void Load(const Ref<ByteBuffer>& buffer);

    };

    class Assets
    {

    };
}