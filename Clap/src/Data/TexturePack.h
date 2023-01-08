// Disclaimer: Algorhithm chosen with the help of this article:
// https://www.david-colson.com/2020/03/10/exploring-rect-packing.html

#pragma once
#include "Core/Core.h"
#include "Data/ByteBuffer.h"

namespace Clap
{
    struct PackedRect
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t* x;
        uint32_t* y;

        byte* data;

        bool IsPacked = false;
    };

    class TexturePacker
    {
    public:
        static void Pack(Ref<ByteBuffer> texture, uint32_t width, uint32_t height, size_t bytesPerPixel, std::vector<PackedRect>& rects);
    };
}