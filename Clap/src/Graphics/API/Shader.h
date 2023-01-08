#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "UniformBuffer.h"
#include "Core/Core.h"
#include "Data/ByteBuffer.h"

namespace Clap
{
    class Shader
    {
    public:
        virtual ~Shader() = default;
        
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetUniformBufferBinding(Ref<UniformBuffer> buffer, const std::string& name) = 0;
        
        virtual const std::string& GetPath() = 0;
        virtual void Reload() = 0;
        virtual void Reload(const std::string& filepath) = 0;
        virtual void Reload(const Ref<ByteBuffer>& buffer) = 0;
        
        virtual void SetBool(const std::string& name, bool value)               = 0;
        virtual void SetInt(const std::string& name, int value)                 = 0;
        virtual void SetIntPtr(const std::string& name, int count, int* value)  = 0;
        virtual void SetFloat(const std::string& name, float value)             = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& mat)     = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& mat)     = 0;
        virtual void SetMat2(const std::string& name, const glm::mat2& mat)     = 0;
        virtual void SetVec4(const std::string& name, const glm::vec4& value)   = 0;
        virtual void SetVec3(const std::string& name, const glm::vec3& value)   = 0;
        virtual void SetVec2(const std::string& name, const glm::vec2& value)   = 0;

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const Ref<ByteBuffer>& buffer);

        //static AssetType GetAssetType() { return AssetType::Shader; }
        //static Ref<Shader> Create(const TextFile& file);
    };
}
