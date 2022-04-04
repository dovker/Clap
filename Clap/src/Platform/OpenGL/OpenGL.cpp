#include "pch.h"

#include "OpenGL.h"
#include "Graphics/API/API.h"
#include "glad/glad.h"

namespace Clap
{
    void OpenGLCheckError(const char *file, int line)
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
                case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
                case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
                case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            CLAP_ERROR( error, " | ", file, " (", line, ")");
        }
    }

    void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         CLAP_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       CLAP_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          CLAP_ERROR(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: CLAP_INFO(message); return;
		}
		
		CLAP_ASSERT(false, "Unknown severity level!");
	}

    void Graphics::Init()
    {
        #if defined(CLAP_DEBUG) && defined(CLAP_OPENGL_4_5)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLMessageCallback, NULL);
            
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        #endif

        glEnable(GL_DEPTH_TEST);
        CheckGPUError();
        glEnable(GL_BLEND);
        CheckGPUError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CheckGPUError();
    }

    void Graphics::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void Graphics::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Graphics::SetBlendingMode(BlendingMode mode)
    {
        switch(mode)
        {
            case BlendingMode::ADDITIVE:
                glBlendFunc(GL_ONE, GL_ONE);
            break;
            case BlendingMode::SUBTRACTIVE:
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            break;
            case BlendingMode::REGULAR:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
            default: break;
        }
        CLAP_ASSERT(false, "BLENDING MODE NOT SUPPORTED");
    }

    void Graphics::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Graphics::ToggleDepthTest(bool toggle)
    {
        toggle ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }

    void Graphics::ToggleBlending(bool toggle)
    {
        toggle ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    }

    void Graphics::ToggleBackfaceCulling(bool toggle)
    {
        toggle ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    }

    void Graphics::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        CheckGPUError();
        glBindTexture(GL_TEXTURE_2D, 0);
        CheckGPUError();
    }

    void Graphics::DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, uint32_t indexCount)
    {
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, instanceCount);
        CheckGPUError();
        glBindTexture(GL_TEXTURE_2D, 0);
        CheckGPUError();
    }

    GLenum GraphicsDataTypeToOpenGLBaseType(GraphicsDataType type)
    {
        switch (type)
        {
            case GraphicsDataType::FLOAT:    return GL_FLOAT;
            case GraphicsDataType::FLOAT2:   return GL_FLOAT;
            case GraphicsDataType::FLOAT3:   return GL_FLOAT;
            case GraphicsDataType::FLOAT4:   return GL_FLOAT;
            case GraphicsDataType::MAT3:     return GL_FLOAT;
            case GraphicsDataType::MAT4:     return GL_FLOAT;
            case GraphicsDataType::INT:      return GL_INT;
            case GraphicsDataType::INT2:     return GL_INT;
            case GraphicsDataType::INT3:     return GL_INT;
            case GraphicsDataType::INT4:     return GL_INT;
            case GraphicsDataType::BOOL:     return GL_BOOL;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown GraphicsDataType!");
        return 0;
    }

    GLenum ToOpenGLInternalFormat(TextureFormat type)
    {
        switch (type)
        {
            case TextureFormat::RGBA8:           return GL_RGBA8;
            case TextureFormat::RGB8:            return GL_RGB8;
            case TextureFormat::R8:              return GL_R8;
            case TextureFormat::RGBA16F:         return GL_RGBA16F;
            case TextureFormat::RGB16F:          return GL_RGB16F;
            case TextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }

    GLenum ToOpenGLDataFormat(TextureFormat type)
    {
        switch (type)
        {
            case TextureFormat::RGBA8:           return GL_RGBA;
            case TextureFormat::RGB8:            return GL_RGB;
            case TextureFormat::R8:              return GL_RED;
            case TextureFormat::RGBA16F:         return GL_RGBA;
            case TextureFormat::RGB16F:          return GL_RGB;
            case TextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }

    GLenum ToOpenGLDataType(TextureFormat type)
    {
        switch (type)
        {
            case TextureFormat::RGBA8:           return GL_UNSIGNED_BYTE;
            case TextureFormat::RGB8:            return GL_UNSIGNED_BYTE;
            case TextureFormat::R8:              return GL_UNSIGNED_BYTE;
            case TextureFormat::RGBA16F:         return GL_FLOAT;
            case TextureFormat::RGB16F:          return GL_FLOAT;
            case TextureFormat::DEPTH24STENCIL8: return GL_UNSIGNED_INT_24_8;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown TextureFormat!");
        return 0;
    }

    GLenum ShaderTypeToOpenGLBaseType(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::FRAGMENT:         return GL_FRAGMENT_SHADER;
            case ShaderType::VERTEX:           return GL_VERTEX_SHADER;
            case ShaderType::GEOMETRY:         return GL_GEOMETRY_SHADER;
            case ShaderType::COMPUTE:          return GL_COMPUTE_SHADER;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown ShaderType!");
        return 0;
    }

    GLenum FilterTypeToOpenGLBaseType(FilterType type)
    {
        switch (type)
        {
            case FilterType::NEAREST:         return GL_NEAREST;
            case FilterType::LINEAR:          return GL_LINEAR;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown FilterType!");
        return 0;
    }

    GLenum WrapTypeToOpenGLBaseType(WrapType type)
    {
        switch (type)
        {
            case WrapType::REPEAT:           return GL_REPEAT;
            case WrapType::CLAMP_TO_EDGE:    return GL_CLAMP_TO_EDGE;
            case WrapType::MIRRORED_REPEAT:  return GL_MIRRORED_REPEAT;
            default: break;
        }

        CLAP_ASSERT(false, "Unknown FilterType!");
        return 0;
    }
}