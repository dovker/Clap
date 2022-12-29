#pragma once

#include "Types.h"
#include "Core/Core.h"
#include "Core/Log.h"

namespace Clap
{

    static size_t TextureFormatToSize(TextureFormat type)
    {
        switch (type)
        {
            case TextureFormat::RGBA8:           return 4;
            case TextureFormat::RGB8:            return 3;
            case TextureFormat::R8:              return 1;
            case TextureFormat::RGBA16F:         return 4 * 4;
            case TextureFormat::RGB16F:          return 3 * 4;
            case TextureFormat::INT32:           return 4;
            case TextureFormat::UINT32:          return 4;
            case TextureFormat::DEPTH24STENCIL8: return 4;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }
}