#pragma once

namespace Clap
{
    enum class TextureFormat
    {
        NONE = 0,

        // Color
        RGBA8,
        RGB8,
        R8,
        RGBA16F,
        RGB16F,
        // Depth/stencil
        DEPTH24STENCIL8,

        // Integer
        INT32,
        UINT32,

        // Defaults
        Depth = DEPTH24STENCIL8
    };

    enum class BlendingMode
    {
        NONE = 0,
        ADDITIVE,
        SUBTRACTIVE,
        REGULAR
    };

    enum class ShaderType
    {
        NONE = -1,
        VERTEX,
        FRAGMENT,
        COMPUTE
    };

    enum class FilterType
    {
        NONE = 0,
        NEAREST,
        LINEAR,
    };

    enum class WrapType
    {
        NONE = 0,
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
    };

    enum class GraphicsDataType
    {
        NONE = 0, 
        FLOAT, 
        FLOAT2,
        FLOAT3, 
        FLOAT4, 
        MAT3, 
        MAT4, 
        INT, 
        INT2, 
        INT3, 
        INT4, 
        BOOL
    };

    struct TextureSpecification
    {
        TextureSpecification() = default;
        TextureSpecification(TextureFormat format, FilterType magFilter = FilterType::NEAREST, FilterType minFilter = FilterType::LINEAR, WrapType wrap = WrapType::CLAMP_TO_EDGE)
            : Format(format), MagFilter(magFilter), MinFilter(minFilter), Wrap(wrap) { }

        TextureFormat Format = TextureFormat::NONE;
        FilterType MagFilter = FilterType::NONE;
        FilterType MinFilter = FilterType::NONE;
        WrapType Wrap = WrapType::NONE;
        bool GenerateMipmaps = false;
        bool AnisotropicFilter = false;
    };
}