#pragma once

namespace Clap
{
    enum class Codepoints
    {

    }
    enum class FontType
    {
        BITMAP = 0,
        SDF,
        MSDF, // Based on 

    }
    struct Glyph
    {
        uint32_t Character; //UTF-8
        uint32_t Width;
        uint32_t Height;
        uint32_t OffsetX;
        uint32_t OffsetY;
    };

    class Font
    {
    public:

    private:
        Ref<Texture2D> m_Bitmap;

        bool m_SDF = false;
    };
}