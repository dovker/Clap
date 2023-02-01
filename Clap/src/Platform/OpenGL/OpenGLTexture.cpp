#include "pch.h"

#include "Data/ByteBuffer.h"
#include "Core/Core.h"
#include "Graphics/API/Utils.h"

#include "OpenGLTexture.h"
#include "stb_image.h"

#include <glad/glad.h>
#include "OpenGL.h"

namespace Clap
{
    void OpenGLTexture2D::m_LoadTextureFromMemory(const Ref<ByteBuffer>& buffer)
    {
        int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load_from_memory(buffer->GetData().data(), buffer->GetData().size(), &width, &height, &channels, 0);
	
		CLAP_ASSERT(data, "Failed to load image! " + m_Path + " stb_image: " + stbi_failure_reason());

        m_LoadTexture(data, width, height, channels);
    }

    OpenGLTexture2D::OpenGLTexture2D(const Ref<ByteBuffer>& buffer)
	{
		m_LoadTextureFromMemory(buffer);
	}

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		Ref<ByteBuffer> buffer = ByteBuffer::Create(path);
        m_LoadTextureFromMemory(buffer);
	}

	
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureSpecification specification)
		: m_Width(width), m_Height(height), m_Specification(specification)
	{
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
        glTextureStorage2D(m_ID, 1, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height);

        m_SetInternalSpecification();
	}


	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

    void OpenGLTexture2D::SetSpecification(TextureSpecification specification)
    {
        bool genMipMaps = specification.GenerateMipmaps && !m_Specification.GenerateMipmaps;
        m_Specification = specification;
        
        m_SetInternalSpecification(genMipMaps);
    }
    void OpenGLTexture2D::m_SetInternalSpecification(bool generateNewMipmaps)
    {
        glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MinFilter));
        glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MagFilter));

        glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapTypeToOpenGLBaseType(m_Specification.Wrap));
        glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapTypeToOpenGLBaseType(m_Specification.Wrap));

        if(generateNewMipmaps && m_Specification.GenerateMipmaps)
        {
            m_GenerateMipmaps(m_Specification.AnisotropicFilter);
        }

    }


    void OpenGLTexture2D::m_GenerateMipmaps(bool anisotropy)
    {
        glGenerateTextureMipmap(m_ID);
        glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameterf(m_ID, GL_TEXTURE_LOD_BIAS, anisotropy ? 0.0f : -1.0f);

        if(GLAD_GL_EXT_texture_filter_anisotropic)
        {
            float maxAnisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

            float value = (CLAP_ANISOTROPY_AMOUNT > maxAnisotropy) ? maxAnisotropy : CLAP_ANISOTROPY_AMOUNT;

            glTextureParameterf(m_ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
        }
        else
        {
            CLAP_ASSERT(false, "OPENGL: Anisotropic filtering not supported");
        }
    }

    void OpenGLTexture2D::m_LoadTexture(unsigned char* data, int width, int height, int channels)
	{
		m_Width = width;
		m_Height = height;

        TextureFormat format;

		if (channels == 4)
		{
			format = TextureFormat::RGBA8;
		}
		else if (channels == 3)
		{
			format = TextureFormat::RGB8;
		}
        else if (channels == 1)
        {
            format = TextureFormat::R8;
        }

        if(m_Specification.Format == TextureFormat::NONE)
        {
            m_Specification = TextureSpecification(format);
        }
        else
            m_Specification.Format = format;


        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
        glTextureStorage2D(m_ID, 1, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height);
        
        m_SetInternalSpecification();

        glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, 
                    ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data);

		stbi_image_free(data);
	}

    void OpenGLTexture2D::Reload(const Ref<ByteBuffer>& buffer)
    {
        m_LoadTextureFromMemory(buffer);
    }
    void OpenGLTexture2D::Reload()
    {
        if(m_Path != "")
        {
            Ref<ByteBuffer> buffer = ByteBuffer::Create(m_Path);
            m_LoadTextureFromMemory(buffer);
        }
        else
        {
            CLAP_ASSERT(false, "CANNOT RELOAD IMAGE, THERE'S NO PATH SET!");
        }
    }

    void FlipTexture(byte* data, uint32_t width, uint32_t height, uint32_t bpp)
    {
        uint32_t rowsSwapCount = height / 2;
        uint32_t maxRowIndex = height - 1;

        for (uint32_t i = 0; i < rowsSwapCount; ++i) {
            for (uint32_t j = 0; j < width; ++j) {
                for(uint32_t k = 0; k < bpp; k++)
                {
                    uint32_t currentDataIndex = width * bpp * i + j + k;
                    uint32_t swapDataIndex = width * bpp * (maxRowIndex - i) + j + k;
                    
                    byte temp = data[currentDataIndex];
                    data[currentDataIndex] = data[swapDataIndex];
                    data[swapDataIndex] = temp;
                }
            }
        }
    }

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = TextureFormatToSize(m_Specification.Format);
		CLAP_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        FlipTexture((byte*)data, m_Width, m_Height, bpp);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data);
	}
    void OpenGLTexture2D::SetData(Ref<ByteBuffer> data)
    {
        uint32_t bpp = TextureFormatToSize(m_Specification.Format);
		CLAP_ASSERT(data->GetData().size() == m_Width * m_Height * bpp, "Data must be entire texture!");
        FlipTexture(data->GetData().data(), m_Width, m_Height, bpp);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data->GetData().data());
    }

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
        glBindTextureUnit(slot, m_ID);
	}
}
