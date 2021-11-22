#include "pch.h"

#include "Shader.h"
#include "Core/Core.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Clap
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        #ifdef CLAP_USE_OPENGL
            return CreateRef<OpenGLShader>(filepath);
        #else
            CLAP_ASSERT(false, "No graphics API set!");
            return nullptr;
        #endif
    }
    // Ref<Shader> Shader::Create(const TextFile& file)
    // {
    //     #ifdef CLAP_USE_OPENGL
    //         return CreateRef<OpenGLShader>(file);
    //     #else
    //         CLAP_ASSERT(false, "No graphics API set!");
    //         return nullptr;
    //     #endif
    // }
}
