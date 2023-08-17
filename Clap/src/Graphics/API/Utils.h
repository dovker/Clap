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
            case TextureFormat::RG8:             return 2;
            case TextureFormat::R8:              return 1;
            case TextureFormat::RGBA16F:         return 4 * 4;
            case TextureFormat::RG16F:           return 2 * 4;
            case TextureFormat::R16F:            return 4;
            case TextureFormat::INT32:           return 4;
            case TextureFormat::UINT32:          return 4;
            case TextureFormat::DEPTH24STENCIL8: return 4;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }
    
    static size_t GraphicsDataTypeSize(GraphicsDataType type)
    {
        switch (type) {
            case GraphicsDataType::FLOAT:     return 4;
            case GraphicsDataType::FLOAT2:    return 4 * 2;
            case GraphicsDataType::FLOAT3:    return 4 * 3;
            case GraphicsDataType::FLOAT4:    return 4 * 4;
            case GraphicsDataType::MAT3:      return 4 * 3 * 3;
            case GraphicsDataType::MAT4:      return 4 * 4 * 4;
            case GraphicsDataType::INT:       return 4;
            case GraphicsDataType::INT2:      return 4 * 2;
            case GraphicsDataType::INT3:      return 4 * 3;
            case GraphicsDataType::INT4:      return 4 * 4;
            case GraphicsDataType::BOOL:      return 1;
            default: break;
        }
        CLAP_ASSERT(false, "Unknown Graphics data type");
        return 0;
    }
}