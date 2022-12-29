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
    OpenGLTexture2D::OpenGLTexture2D(const Ref<ByteBuffer>& buffer)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load_from_memory(buffer->GetData().data(), buffer->GetData().size(), &width, &height, &channels, 0);
	
		CLAP_ASSERT(data, "Failed to load image! " + m_Path + " stb_image: " + stbi_failure_reason());

		m_LoadTexture(data, width, height, channels);
	}

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	
		CLAP_ASSERT(data, "Failed to load image! " + m_Path + " stb_image: " + stbi_failure_reason());

		m_LoadTexture(data, width, height, channels);
	}

	
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureSpecification specification)
		: m_Width(width), m_Height(height), m_Specification(specification)
	{
        #ifdef CLAP_OPENGL_4_5
           glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
           glTextureStorage2D(m_ID, 1, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height);

           m_SetInternalSpecification();
        #else
            glGenTextures(1, &m_ID);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_ID);
            CheckGPUError();
            
            m_SetInternalSpecification();
            
            glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height, 0, 
                    ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), nullptr);
            CheckGPUError();
                        
            glBindTexture(GL_TEXTURE_2D, 0);  
        #endif      
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
        #ifdef CLAP_OPENGL_4_5
            glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MinFilter));
            glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MagFilter));

            glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, WrapTypeToOpenGLBaseType(m_Specification.Wrap));
            glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, WrapTypeToOpenGLBaseType(m_Specification.Wrap));
        #else
            glBindTexture(GL_TEXTURE_2D, m_ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MinFilter));
            CheckGPUError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterTypeToOpenGLBaseType(m_Specification.MagFilter));
            CheckGPUError();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapTypeToOpenGLBaseType(m_Specification.Wrap));
            CheckGPUError();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapTypeToOpenGLBaseType(m_Specification.Wrap));
            CheckGPUError();
        #endif

        if(generateNewMipmaps && m_Specification.GenerateMipmaps)
        {
            m_GenerateMipmaps(m_Specification.AnisotropicFilter);
        }

    }


    void OpenGLTexture2D::m_GenerateMipmaps(bool anisotropy)
    {
        #ifdef CLAP_OPENGL_4_5
            glGenerateTextureMipmap(m_ID);
            glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameterf(m_ID, GL_TEXTURE_LOD_BIAS, anisotropy ? 0.0f : -1.0f);
        #else
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            CheckGPUError();
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, anisotropy ? 0.0f : -1.0f);
            CheckGPUError();
        #endif

        if(GLAD_GL_EXT_texture_filter_anisotropic)
        {
            float maxAnisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

            float value = (CLAP_ANISOTROPY_AMOUNT > maxAnisotropy) ? maxAnisotropy : CLAP_ANISOTROPY_AMOUNT;
            #ifdef CLAP_OPENGL_4_5
                glTextureParameterf(m_ID, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
            #else
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
                CheckGPUError();
            #endif
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

        #ifdef CLAP_OPENGL_4_5
            glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
            glTextureStorage2D(m_ID, 1, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height);
            
            m_SetInternalSpecification();

            glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, 
                        ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data);
        #else
            glGenTextures(1, &m_ID);
            glBindTexture(GL_TEXTURE_2D, m_ID);
            CheckGPUError();
            
            m_SetInternalSpecification();
            
            glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLInternalFormat(m_Specification.Format), m_Width, m_Height, 0, 
                        ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data);
            CheckGPUError();
            
            glBindTexture(GL_TEXTURE_2D, 0);
        #endif
        
		stbi_image_free(data);
	}

    void OpenGLTexture2D::Reload(const std::string& path)
    {
        m_Path = path;
        int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	
		CLAP_ASSERT(data, "Failed to load image! " + m_Path + " stb_image: " + stbi_failure_reason());

        if(m_ID) glDeleteTextures(1, &m_ID);

		m_LoadTexture(data, width, height, channels);
    }
    void OpenGLTexture2D::Reload()
    {
        if(m_Path != "")
        {
            int width, height, channels;
            stbi_set_flip_vertically_on_load(1);

            unsigned char* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
        
            CLAP_ASSERT(data, "Failed to reload image! " + m_Path + " stb_image: " + stbi_failure_reason());

            if(m_ID) glDeleteTextures(1, &m_ID);

            m_LoadTexture(data, width, height, channels);
        }
        else
        {
            CLAP_ASSERT(false, "CANNOT RELOAD IMAGE, THERE'S NO PATH SET!");
        }
    }

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = TextureFormatToSize(m_Specification.Format);
		CLAP_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");

        #ifdef CLAP_OPENGL_4_5
		    glTexSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, ToOpenGLDataFormat(m_Specification.Format), ToOpenGLDataType(m_Specification.Format), data);
        #else
            glBindTexture(GL_TEXTURE_2D, m_ID);
            glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLInternalFormat(m_Specification.Format), 
                        m_Width, m_Height, 0, ToOpenGLDataFormat(m_Specification.Format), 
                        ToOpenGLDataType(m_Specification.Format), data);
            CheckGPUError();
            glBindTexture(GL_TEXTURE_2D, 0);
        #endif
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
        #ifdef CLAP_OPENGL_4_5
            glBindTextureUnit(slot, m_ID);
        #else
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_ID);
            CheckGPUError();
        #endif
	}
}
