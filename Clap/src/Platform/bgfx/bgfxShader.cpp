#include "pch.h"

#include "Core/Log.h"
#include "Core/Core.h"

#include "Graphics/API/Types.h"

#include "bgfxShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Clap
{
	
	bgfxShader::bgfxShader(const std::string& path)
		: m_Path(path)
	{
		
	}

	uint32_t bgfxShader::CompileShader(unsigned int type, const std::string& source)
	{
		return 0;
	}

	uint32_t bgfxShader::Compile(const std::string& text)
	{
		return 0;
	}
    void bgfxShader::Recompile()
    {
        
    }
	void bgfxShader::RecompileSource(const std::string& source)
    {
		
    }
	void bgfxShader::Recompile(const std::string& filepath)
	{
		
	}
	void bgfxShader::SetUniformBufferBinding(Ref<UniformBuffer> buffer, const std::string& name)
	{
		
	}

	void bgfxShader::Bind()
	{
		
	}
    void bgfxShader::Unbind()
    {
        
    }

	void bgfxShader::SetBool(const std::string& name, bool value)
	{
		
	}
	void bgfxShader::SetInt(const std::string& name, int value)
	{
		
	}
	void bgfxShader::SetIntPtr(const std::string& name, int count, int* value)
	{

	}
	void bgfxShader::SetFloat(const std::string& name, float value)
	{
		
    }
	void bgfxShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		
	}
	void bgfxShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		
	}
	void bgfxShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		
	}
	void bgfxShader::SetMat2(const std::string& name, const glm::mat2& mat)
	{
		
	}
	void bgfxShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		
	}
	void bgfxShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		
	}
}
