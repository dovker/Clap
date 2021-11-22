#pragma once

#include "glad/glad.h"
#include "Graphics/API/Types.h"

namespace Clap
{
    static GLenum GraphicsDataTypeToOpenGLBaseType(GraphicsDataType type);
    static GLenum ToOpenGLInternalFormat(TextureFormat type);
    static GLenum ToOpenGLDataFormat(TextureFormat type);
    static GLenum ToOpenGLDataType(TextureFormat type);
    static GLenum ShaderTypeToOpenGLBaseType(ShaderType type);
    static GLenum FilterTypeToOpenGLBaseType(FilterType type);
    static GLenum WrapTypeToOpenGLBaseType(WrapType type);
}