#pragma once
#include "Core/Core.h"
#include "Core/Log.h"
#include "Data/ByteBuffer.h"
#include "Graphics/API/Texture.h"

namespace Clap
{
    #define TEXTURE_PACK_SIZE 2048
    
    enum class FontType
    {
        BITMAP = 0,
        SDF,
        MSDF,
        SPRITEFONT,
    };
    struct GlyphData
    {
        uint32_t PosX;
        uint32_t PosY;
        uint32_t Width;
        uint32_t Height;
        float OffsetX;
        float OffsetY;
        float Advance;
    };

    class Font
    {
    public:
        Font(const Ref<ByteBuffer>& file);
        Font(const std::string filePath);
        Font(const Ref<Texture2D>& texture, uint32_t charWidth, uint32_t charHeight, std::string characters);

        void Generate(uint32_t fontHeight, FontType fontType, bool kerning, uint32_t codepointFrom = 0, uint32_t codepointTo = 127, 
                 FilterType filterType = FilterType::LINEAR, uint32_t atlasWidth = TEXTURE_PACK_SIZE, uint32_t atlasHeight = TEXTURE_PACK_SIZE);

        Ref<Texture2D> GetTexture() { return m_Atlas; }

        //TODO: RELOAD

        static Ref<Font> Create(const Ref<ByteBuffer>& file);
        static Ref<Font> Create(const std::string& filepath);
        static Ref<Font> Create(const Ref<Texture2D>& texture, uint32_t charWidth, uint32_t charHeight, std::string characters);
    private:
        std::string m_Path;
        Ref<ByteBuffer> m_Data;

        FontType m_FontType      = FontType::BITMAP;
        uint32_t m_CodepointFrom = 0; //Unicode Codepoint start
        uint32_t m_CodepointTo   = 127; //Unicode Codepoint end
        uint32_t m_Height        = 32;
        bool     m_Kerning       = false;
        float    m_Ascent, m_Descent, m_LineGap;
        std::unordered_map<uint32_t, GlyphData> m_Glyphs; //Character -> Glyph Info
        //std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t> m_KerningTable; // Pair(Char1, Char2) -> Advance of pixels

        Ref<Texture2D> m_Atlas;
    };

    // enum class Codepoints
    // {

    // }
}