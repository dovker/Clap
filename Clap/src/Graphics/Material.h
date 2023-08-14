#pragma once
#include "pch.h"

#include "Core/Core.h"
#include "Data/ByteBuffer.h"
#include "API/Texture.h"
#include "API/Shader.h"
#include "API/Utils.h"

#include <glm/glm.hpp>


namespace Clap
{
    struct UniformValue
    {
        GraphicsDataType Type;
        std::string Name;

        UniformValue() {};
        UniformValue(GraphicsDataType type, const std::string name)
            : Name(name), Type(type)
        {}
    };

    struct PBRMaterialUniform
    {
        glm::vec4 Albedo = {1.0f, 1.0f, 1.0f, 1.0f};
        float Emissive = 0.0f;
        float Metallic = 0.0f;
        float Roughness = 1.0f;

        std::vector<UniformValue> GetUniformLayout()
        {
            std::vector<UniformValue> layout 
            {
                {GraphicsDataType::FLOAT4, "Albedo"},
                {GraphicsDataType::FLOAT,  "Emissive"}, 
                {GraphicsDataType::FLOAT,  "Metallic"}, 
                {GraphicsDataType::FLOAT,  "Roughness"} 
            };

            return layout;
        }

        PBRMaterialData() {}
    };
    
    class Material
    {
    public:
        Material(std::vector<UniformValue> layout, Ref<Shader> shader = nullptr);
        Material(PBRMaterialUniform data, Ref<Shader> shader = nullptr);
        Material(void* data, std::vector<UniformValue> layout, Ref<Shader> shader = nullptr);

        ~Material();

        Ref<Shader> GetShader() { return m_Shader; }
        void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }

        Ref<Texture2D> GetTexture(std::string name);
        std::map<std::string, Ref<Texture2D>> GetTextures() { return m_Textures; }
        void SetTexture(std::string name, const Ref<Texture2D>& texture) { m_Textures[name] = texture; }

        template<typename T>
        T GetValue(const std::string name)
        {
            CLAP_ASSERT(m_Offsets.find(name) != m_Offsets.end(), "Such member of material does not exist");

            return *(T*)(data+m_Offsets[name]);
        } 
        template<typename T>
        void SetValue(const std::string name, T value)
        {
            CLAP_ASSERT(m_Offsets.find(name) != m_Offsets.end(), "Such member of material does not exist");

            *(T*)(data+m_Offsets[name]) = value;
        } 
        template<typename T>
        T GetData()
        {
            CLAP_ASSERT(sizeof(T) == m_Size, "Material Size is not equal to the size of presented type");
            return *(T*)(data);
        }

        std::vector<UniformValue> GetLayout() { return m_Layout; }
        void* GetData() { return m_Data; }
        void SetData(void* data);
        size_t GetSize() { return m_Size; }

        static Ref<Material> Create(std::vector<UniformValue> layout, Ref<Shader> shader = nullptr);
        static Ref<Material> Create(PBRMaterialUniform data, Ref<Shader> shader = nullptr);
        static Ref<Material> Create(void* data, std::vector<UniformValue> layout, Ref<Shader> shader = nullptr);

    private:
        Ref<Shader> m_Shader;
        std::map<std::string, Ref<Texture2D>> m_Textures;
        std::vector<UniformValue> m_Layout;
        std::unordered_map<std::string, uint32_t> m_Offsets; //string - name, uint32_t - offset

        void* m_Data;
        size_t m_Size;

        void m_CalculateSizeAndOffsets();
    };
} 