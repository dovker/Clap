#pragma once

#include "glad/glad.h"
#include "Graphics/API/Types.h"


namespace Clap
{
    void OpenGLCheckError(const char *file, int line);
    GLenum GraphicsDataTypeToOpenGLBaseType(GraphicsDataType type);
    GLenum ToOpenGLInternalFormat(TextureFormat type);
    GLenum ToOpenGLDataFormat(TextureFormat type);
    GLenum ToOpenGLDataType(TextureFormat type);
    GLenum ShaderTypeToOpenGLBaseType(ShaderType type);
    GLenum FilterTypeToOpenGLBaseType(FilterType type);
    GLenum WrapTypeToOpenGLBaseType(WrapType type);
}

#if defined(CLAP_DEBUG) && !defined(CLAP_OPENGL_4_5) && defined(CLAP_DEBUG_GRAPHICS)
    #define CheckGPUError()  OpenGLCheckError(__FILE__, __LINE__) 
#else
    #define CheckGPUError()
#endif