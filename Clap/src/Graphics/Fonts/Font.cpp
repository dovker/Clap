#include "pch.h"
#include "Font.h"
#include "Data/TexturePack.h"

#include "stb_truetype.h"
#include "utf8.h"


namespace Clap
{
    Font::Font(const Ref<ByteBuffer>& file)
        : m_Data(file)
    {}

    Font::Font(const std::string filePath)
        : m_Path(filePath)
    {
        Ref<ByteBuffer> buffer = ByteBuffer::Create(filePath);
        m_Data = buffer;
    }

    Font::Font(const Ref<Texture2D>& texture, uint32_t charWidth, uint32_t charHeight, std::string characters)
        : m_Atlas(texture)
    {
        uint32_t tW = texture->GetWidth();

        uint32_t codepoint;
        const char* charPointer = characters.c_str();

        uint32_t j = 0;

        int i = 0;
        for(int i = 0; i < characters.length();)
        {
            charPointer = utf8codepoint(charPointer, (utf8_int32_t*)&codepoint);
            i = charPointer - characters.c_str();

            float sY = ((uint32_t)(j * charHeight) / tW);
            float sX = ((uint32_t)(j * charWidth) % tW) / charWidth;
            
            GlyphData glyphData = { sX * charWidth, sY * charHeight, (uint32_t)charWidth, (uint32_t)charHeight, 0, 0, (float)charWidth };
            m_Glyphs.insert({codepoint, glyphData});

            j++;
        }
        m_Ascent = charHeight;
        m_Descent = 0.0f;
        m_LineGap = 0.0f;
    }

    void Font::Generate(uint32_t fontHeight, FontType fontType, bool kerning, uint32_t codepointFrom, uint32_t codepointTo, 
                   FilterType filterType, uint32_t atlasWidth, uint32_t atlasHeight)
    {
        m_Height = fontHeight;
        m_FontType = fontType;
        m_Kerning = kerning;
        m_CodepointFrom = codepointFrom;
        m_CodepointTo = codepointTo;

        uint32_t channels = 1;
        
        switch(m_FontType)
        {
            case FontType::BITMAP:  channels = 1; break;
            case FontType::SDF:     channels = 1; break;
            default:
                CLAP_ASSERT(false, "Unsupported Font Type!");
            break;
        }

        uint32_t numberOfChars = codepointTo - codepointFrom;

        Ref<ByteBuffer> texData = ByteBuffer::Create(atlasWidth * atlasHeight * channels);

        stbtt_fontinfo font;
        stbtt_InitFont(&font, m_Data->GetData().data(), 0);

        float scale = stbtt_ScaleForPixelHeight(&font, (float)fontHeight);
        
        int stbAscent;
        int stbDescent;
        int stbLineGap;
        stbtt_GetFontVMetrics(&font, &stbAscent, &stbDescent, &stbLineGap);
        
        m_Ascent  = (float)stbAscent  * scale;
        m_Descent = (float)stbDescent * scale;
        m_LineGap = (float)stbLineGap * scale;

        switch(m_FontType)
        {
            case FontType::BITMAP: 
            {
                stbtt_bakedchar* charData = new stbtt_bakedchar[numberOfChars];
                stbtt_BakeFontBitmap(m_Data->GetData().data(), 0, (float)fontHeight, texData->GetData().data(), atlasWidth, atlasHeight, codepointFrom, numberOfChars, charData);

                for(uint32_t i = 0; i < numberOfChars; i++)
                {
                    stbtt_bakedchar glyph = charData[i];
                    m_Glyphs[i + codepointFrom] = {glyph.x0, glyph.y0, (uint32_t)(glyph.x1 - glyph.x0), (uint32_t)(glyph.y1 - glyph.y0), glyph.xoff, glyph.yoff, glyph.xadvance};
                }

                free(charData);
            }
            break;

            case FontType::SDF:
            {
                std::vector<PackedRect> rects;
                rects.reserve(numberOfChars);
                for(uint32_t glyph = codepointFrom; glyph < codepointTo; glyph++)
                {
                    int advance;
                    int leftBearing;
                    stbtt_GetCodepointHMetrics(&font, glyph, &advance, &leftBearing);
                    
                    advance *= scale;
                    leftBearing *= scale;
                    
                    int width, height;
                    int xOffset, yOffset;

                    int padding = 10; //Laaaarge padding, but may be useful.
                    int onedge_value = 128;
                    float pixel_dist_scale = (float)onedge_value / (float)padding;


                    unsigned char* stbBitmap = stbtt_GetCodepointSDF(&font, scale, glyph, padding, onedge_value, pixel_dist_scale,
                                                                   &width, &height, &xOffset, &yOffset);
                    if(width < 0 || width > 10000)
                        width = 0;
                    if(height < 0 || height > 10000)
                        height = 0;

                    GlyphData glyphData = { 0, 0, (uint32_t)width, (uint32_t)height, (float)xOffset, (float)yOffset, (float)advance };
                    m_Glyphs.insert({glyph, glyphData});

                    rects.push_back({(uint32_t)width, (uint32_t)height, &m_Glyphs.at(glyph).PosX, &m_Glyphs.at(glyph).PosY, stbBitmap, false});
                }
                TexturePacker::Pack(texData, atlasWidth, atlasHeight, channels, rects);
                for(PackedRect rect : rects)
                {
                    CLAP_ASSERT(rect.IsPacked, "Not all characters fit!");
                }
            }
            break;

            case FontType::MSDF:
                CLAP_ASSERT(false, "MSDF FONTS ARE NOT YET IMPLEMENTED");
            break;
            
            case FontType::SPRITEFONT:
                CLAP_ASSERT(false, "SPRITE FONTS ARE GENERATED IN THE CONSTRUCTOR");
            break;
        }
        
        if(m_Kerning)
        {
            for(auto &[A, value1] : m_Glyphs)
            {
                for(auto &[B, value2] : m_Glyphs)
                {
                    int ExtractedKern = stbtt_GetCodepointKernAdvance(&font, A, B);
                    
                    m_KerningTable[A][B] = scale * (float)ExtractedKern;
                }
            }
        }
        TextureFormat format = TextureFormat::R8;
        switch(m_FontType)
        {
            case FontType::BITMAP:  format = TextureFormat::R8; break;
            case FontType::SDF:     format = TextureFormat::R8; break;
            default:
                CLAP_ASSERT(false, "Unsupported Font Type!");
            break;
        }

        TextureSpecification texSpec = TextureSpecification(format, filterType, filterType);
        m_Atlas = Texture2D::Create(atlasWidth, atlasHeight, texSpec);
        m_Atlas->SetData(texData);
    }
    float Font::GetWidthPixels(std::string& text, float kerning, float scaleX)
    {
        float advanceX = 0.0f;

        uint32_t codepoint;
        uint32_t nextCodepoint;
        const char* charPointer = text.c_str();

        int i = 0;
        for(int i = 0; i < text.length();)
        {
            charPointer = utf8codepoint(charPointer, (utf8_int32_t*)&codepoint);
            utf8codepoint(charPointer, (utf8_int32_t*)&nextCodepoint);
            i = charPointer - text.c_str();

            if(codepoint == ' ' && advanceX == 0.0f) continue;

            GlyphData data = GetGlyph(codepoint);
            if(nextCodepoint != '\0')
            {
                GlyphData nextData = GetGlyph(nextCodepoint);
                advanceX += (data.Advance + GetKerning(codepoint, nextCodepoint)) * scaleX + kerning;
            }
        }

        return advanceX;
    }

    Ref<Font> Font::Create(const Ref<ByteBuffer>& file)
    {
        return CreateRef<Font>(file);
    }

    Ref<Font> Font::Create(const std::string& filepath)
    {
        return CreateRef<Font>(filepath);
    }

    Ref<Font> Font::Create(const Ref<Texture2D>& texture, uint32_t charWidth, uint32_t charHeight, std::string characters)
    {
        return CreateRef<Font>(texture, charWidth, charHeight, characters);
    }

}
