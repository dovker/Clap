#include "pch.h"
#include "Texture.h"
#include "API.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Clap
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLTexture2D>(path);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureSpecification specification)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLTexture2D>(width, height, specification);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    Ref<Texture2D> Texture2D::Create(const Ref<ByteBuffer>& buffer)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLTexture2D>(buffer);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
}
