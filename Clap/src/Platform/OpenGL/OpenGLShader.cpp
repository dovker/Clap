#include "pch.h"

#include "Core/Log.h"
#include "Core/Core.h"

#include "Graphics/API/Types.h"

#include "OpenGLShader.h"
#include "OpenGL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace Clap
{
	
	OpenGLShader::OpenGLShader(const std::string& path)
		: m_Path(path)
	{
		m_ID = Compile();
	}
	OpenGLShader::OpenGLShader(const Ref<ByteBuffer>& buffer)
	{
		std::string source(buffer->GetData().begin(), buffer->GetData().end());
		m_ID = Compile(source);
	}

	uint32_t OpenGLShader::CompileShader(unsigned int type, const std::string& source)
	{
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
		int result;

		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::string shaderTypeName;
			switch (type)
			{
			case GL_VERTEX_SHADER:
				shaderTypeName = "vertex";
				break;
			case GL_FRAGMENT_SHADER:
				shaderTypeName = "fragment";
				break;
			case GL_COMPUTE_SHADER:
				shaderTypeName = "compute";
				break;
			}

			CLAP_ERROR("Failed to compile ", shaderTypeName, " Shader");
			CLAP_ERROR(message);

			glDeleteShader(id);
			return 0;
		}

		return id;
	}
	
	void _helper_shader_func(std::string& line, std::stringstream ss[], ShaderType& type)
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			else if (line.find("compute") != std::string::npos)
				type = ShaderType::COMPUTE;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	uint32_t OpenGLShader::Compile(const std::string& text)
	{
		bool isFile = false;

		std::stringstream sStream;
		std::ifstream fStream(m_Path);

		if(text.empty())
			isFile = true;
		else
			sStream << text;
		
		std::string line;
		std::stringstream ss[3];
		ShaderType type = ShaderType::NONE;

		if(isFile)
		{
			while (getline(fStream, line))
			{
				_helper_shader_func(line, ss, type);
			}
			CLAP_ASSERT(!fStream.bad(), "SHADER IFSTREAM ERROR: Error Reading the file");
		}
		else
		{
			while (getline(sStream, line))
			{
				_helper_shader_func(line, ss, type);
			}
		}

		const std::string vertexShader   = ss[0].str();
		const std::string fragmentShader = ss[1].str();
		const std::string computeShader = ss[2].str();
		
		uint32_t id = glCreateProgram();
		uint32_t vs = 0, fs = 0, cs = 0;

		if(vertexShader.length() > 0)
		{
			vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
			glAttachShader(id, vs);
		}
		if(fragmentShader.length() > 0)
		{
			fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
			glAttachShader(id, fs);
		}
		if(computeShader.length() > 0)
		{
			cs = CompileShader(GL_COMPUTE_SHADER, computeShader);
			glAttachShader(id, cs);
		}

		glLinkProgram(id);
		glValidateProgram(id);

		if(vs)
			glDeleteShader(vs);
		if(fs)
			glDeleteShader(fs);
		if(cs)
			glDeleteShader(cs);

		return id;
	}
    void OpenGLShader::Reload()
    {
		if(!m_Path.empty())
        	m_ID = Compile();
		else	
			CLAP_ASSERT(false, "Cannot reload a shader, that has no path set");
    }
	void OpenGLShader::Reload(const Ref<ByteBuffer>& buffer)
    {
		std::string source(buffer->GetData().begin(), buffer->GetData().end());
		m_ID = Compile(source);
    }
	void OpenGLShader::Reload(const std::string& filepath)
	{
		m_Path = filepath;
		m_ID = Compile();
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_ID);
	}
    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
	}
	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void OpenGLShader::SetIntPtr(const std::string& name, int count, int* value)
	{
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		glUniform1iv(location, count, value);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}
