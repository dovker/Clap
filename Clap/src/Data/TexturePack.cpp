#include "pch.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "TexturePack.h"

namespace Clap
{
    struct PackedRectSort
    {
        inline bool operator() (const PackedRect& rect1, const PackedRect& rect2)
        {
            return (rect1.Height > rect2.Height);
        }
    };

    void TexturePacker::Pack(Ref<ByteBuffer> texture, uint32_t width, uint32_t height, size_t bytesPerPixel, std::vector<PackedRect>& rects)
    {
        std::sort(rects.begin(), rects.end(), PackedRectSort());

        uint32_t xPos = 0;
        uint32_t yPos = 0;
        uint32_t largestHThisRow = 0;

        for (PackedRect& rect : rects)
        {
            if ((xPos + rect.Width) > width)
            {
                yPos += largestHThisRow;
                xPos = 0;
                largestHThisRow = 0;
            }

            if ((yPos + rect.Height) > height)
            {
                CLAP_ASSERT(false, "COULD NOT FIT INTO THE IMAGE!");
                break;
            }
            
            for(uint32_t y = 0; y < rect.Height; y++)
            {
                for(uint32_t x = 0; x < rect.Width; x++)
                {
                    for(uint32_t i = 0; i < bytesPerPixel; i++)
                    {
                        byte pixel = rect.data[(x + y * rect.Width) * bytesPerPixel + i];
                        texture->GetData()[(xPos + x + (yPos + y) * width) * bytesPerPixel + i ] = pixel;
                    }
                }
            }

            *rect.x = xPos;
            *rect.y = yPos;

            // Move along to the next spot in the row
            xPos += rect.Width;
            
            // Just saving the largest height in the new row
            if (rect.Height > largestHThisRow)
                largestHThisRow = rect.Height;

            // Success!
            rect.IsPacked = true;
        }
    }
}