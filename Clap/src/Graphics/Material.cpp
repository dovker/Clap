#include "pch.h"

#include "Material.h"

namespace Clap
{
    void Material::m_CalculateSizeAndOffsets()
    {
        uint32_t offset = 0;
        for (UniformValue value : m_Layout)
        {
            m_Offsets[value.Name] = offset;
            offset += GraphicsDataTypeSize(value.Type);
        }

        m_Size = offset;
    }

    Material::Material(std::vector<UniformValue> layout, Ref<Shader> shader)
        : m_Shader(shader), m_Layout(layout)
    {
        m_CalculateSizeAndOffsets();
        m_Data = (void*)(new byte[m_Size]);
    }

    Material::Material(PBRMaterialUniform data, Ref<Shader> shader)
        : m_Shader(shader)
    {
        m_Layout = data.GetUniformLayout();
        m_CalculateSizeAndOffsets();

        m_Data = (void*)(new byte[m_Size]);
        memcpy(m_Data, (void*)&data, m_Size);
    }

    Material::Material(void* data, std::vector<UniformValue> layout, Ref<Shader> shader)
        : m_Shader(shader), m_Layout(layout)
    {
        m_CalculateSizeAndOffsets();

        m_Data = (void*)(new byte[m_Size]);
        memcpy(m_Data, data, m_Size);
    }   

    void Material::SetData(void* data)
    {
        memcpy(m_Data, data, m_Size);
    }


    Ref<Texture2D> Material::GetTexture(std::string name)
    {
        if (m_Textures.find(name) == m_Textures.end())
            return nullptr;

        return m_Textures[name];
    }

    Material::~Material()
    {
        delete m_Data;
    }

    Ref<Material> Material::Create(std::vector<UniformValue> layout, Ref<Shader> shader)
    {
        return CreateRef<Material>(layout, shader);
    }

    Ref<Material> Material::Create(PBRMaterialUniform data, Ref<Shader> shader)
    {
        return CreateRef<Material>(data, shader);
    }

    Ref<Material> Material::Create(void* data, std::vector<UniformValue> layout, Ref<Shader> shader)
    {
        return CreateRef<Material>(data, layout, shader);
    }

}