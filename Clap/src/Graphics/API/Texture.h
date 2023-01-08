//MIPMAP SUPPORT
#pragma once

#include "API.h"
#include "Data/ByteBuffer.h"

namespace Clap
{
	class Texture2D
	{
    public:
        virtual ~Texture2D() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetID() const = 0;
        virtual std::string& GetPath() = 0;
        virtual TextureSpecification GetSpecification() = 0;

        virtual void SetData(void* data, uint32_t size) = 0;
        virtual void SetData(Ref<ByteBuffer> data) = 0;
        virtual void SetSpecification(TextureSpecification specification) = 0;

        virtual void Reload(const Ref<ByteBuffer>& buffer) = 0;
        virtual void Reload() = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
        
        virtual bool operator==(const Texture2D& other) const = 0;

        static Ref<Texture2D> Create(const std::string& path);
        static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureSpecification specification);
        static Ref<Texture2D> Create(const Ref<ByteBuffer>& buffer);
        // static AssetType GetAssetType() { return AssetType::Texture; }
	};

}
