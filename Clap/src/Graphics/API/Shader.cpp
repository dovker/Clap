#include "pch.h"

#include "Shader.h"
#include "Core/Core.h"
#include "Core/Log.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Clap
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLShader>(filepath);
        #elif CLAP_USE_BGFX
            return nullptr;
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
}
