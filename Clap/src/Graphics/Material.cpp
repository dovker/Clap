#include "pch.h"

#include "Material.h"

namespace Clap
{
    void Material::m_CalculateSizeAndOffsets()
    {
        uint32_t offset;
        for (UniformValue value : m_Layout)
        {
            m_Offsets[value.Name] = offset;
            offset += GraphicsDataTypeSize(value.Type);
        }

        m_Size = offset;
    }

    Material::Material(const Ref<Shader>& shader, std::vector<UniformValue> layout)
        : m_Shader(shader), m_Layout(layout)
    {
        m_CalculateSizeAndOffsets();
        m_Data = (void*)(new byte[m_Size]);
    }

    Material::Material(const Ref<Shader>& shader, PBRMaterialUniform data)
        : m_Shader(shader)
    {
        m_Layout = data.GetUniformLayout();
        m_CalculateSizeAndOffsets();

        m_Data = (void*)(new byte[m_Size]);
        memcpy(m_Data, (void*)&data, m_Size);
    }

    Material::Material(const Ref<Shader>& shader, void* data, std::vector<UniformValue> layout)
        : m_Shader(shader), m_Layout(layout)
    {
        m_CalculateSizeAndOffsets();

        m_Data = (void*)(new byte[m_Size]);
        memcpy(m_Data, data, m_Size);
    }   


    Ref<Texture2D> Material::GetTexture(std::string name)
    {
        CLAP_ASSERT(m_Textures.find(name) != m_Textures.end(), "Such texture of material does not exist");

        return m_Textures[name];
    }

    Material::~Material()
    {
        delete m_Data;
    }
}