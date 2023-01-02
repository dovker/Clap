#pragma once

#include "Graphics/API/API.h"
#include "Graphics/API/Texture.h"
#include "Data/ByteBuffer.h"

namespace Clap
{
	class OpenGLTexture2D : public Texture2D
	{
    public:
        OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height, TextureSpecification specification);
        OpenGLTexture2D(const Ref<ByteBuffer>& buffer);
        ~OpenGLTexture2D();

        virtual uint32_t GetWidth() const override { return m_Width;  }
        virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetID() const override { return m_ID; }
        virtual std::string& GetPath() override { return m_Path; }
        virtual TextureSpecification GetSpecification() override { return m_Specification; }

        virtual void SetData(void* data, uint32_t size) override;
        virtual void SetSpecification(TextureSpecification specification) override;

        virtual void Reload(const std::string& path) override;
        virtual void Reload() override;

        virtual void Bind(uint32_t slot = 0) const override;
        
        virtual bool operator==(const Texture2D& other) const override
        {
            return m_ID == ((OpenGLTexture2D&)other).m_ID;
        }

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        TextureSpecification m_Specification;
        uint32_t m_ID;
        void m_LoadTextureFromMemory(const Ref<ByteBuffer>& buffer);
        void m_LoadTexture(unsigned char* data, int width, int height, int channels);
        void m_SetInternalSpecification(bool generateNewMipmaps = true);
        void m_GenerateMipmaps(bool anisotropy);
	};

}
