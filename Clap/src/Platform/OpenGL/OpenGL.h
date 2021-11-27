#pragma once

#include "glad/glad.h"
#include "Graphics/API/Types.h"

namespace Clap
{
    GLenum GraphicsDataTypeToOpenGLBaseType(GraphicsDataType type);
    GLenum ToOpenGLInternalFormat(TextureFormat type);
    GLenum ToOpenGLDataFormat(TextureFormat type);
    GLenum ToOpenGLDataType(TextureFormat type);
    GLenum ShaderTypeToOpenGLBaseType(ShaderType type);
    GLenum FilterTypeToOpenGLBaseType(FilterType type);
    GLenum WrapTypeToOpenGLBaseType(WrapType type);
}