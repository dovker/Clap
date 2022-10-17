#include "pch.h"

#include "Data/ByteBuffer.h"
#include "Core/Core.h"

#include "bgfxTexture.h"
#include "stb_image.h"


namespace Clap
{
    bgfxTexture2D::bgfxTexture2D(const Ref<ByteBuffer>& buffer)
	{
		
	}

    bgfxTexture2D::bgfxTexture2D(const std::string& path)
		: m_Path(path)
	{
		
	}

	
	bgfxTexture2D::bgfxTexture2D(uint32_t width, uint32_t height, TextureSpecification specification)
		: m_Width(width), m_Height(height), m_Specification(specification)
	{
        
	}


	bgfxTexture2D::~bgfxTexture2D()
	{
		
	}

    void bgfxTexture2D::SetSpecification(TextureSpecification specification)
    {
        
    }
    void bgfxTexture2D::m_SetInternalSpecification(bool generateNewMipmaps)
    {

    }


    void bgfxTexture2D::m_GenerateMipmaps(bool anisotropy)
    {
        
    }

    void bgfxTexture2D::m_LoadTexture(unsigned char* data, int width, int height, int channels)
	{
		
	}

    void bgfxTexture2D::Reload(const std::string& path)
    {
        
    }
    void bgfxTexture2D::Reload()
    {
        
    }

    size_t FormatToSize(TextureFormat type)
    {
        switch (type)
        {
            case TextureFormat::RGBA8:           return 4;
            case TextureFormat::RGB8:            return 3;
            case TextureFormat::R8:              return 1;
            case TextureFormat::RGBA16F:         return 4 * 4;
            case TextureFormat::RGB16F:          return 3 * 4;
            case TextureFormat::DEPTH24STENCIL8: return 4;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }

	void bgfxTexture2D::SetData(void* data, uint32_t size)
	{
		
	}

	void bgfxTexture2D::Bind(uint32_t slot) const
	{
        
	}
}
