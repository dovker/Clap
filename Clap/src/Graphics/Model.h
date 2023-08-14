#pragma once

#include "Core/Core.h"
#include "Material.h"
#include "Mesh.h"
#include "Data/ByteBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Clap
{
    class Model
    {
    public:
        Model(Ref<Mesh> mesh, Ref<Material> material);
        Model(const std::string path);
        Model(const Ref<ByteBuffer>& buffer);

        std::unordered_map<std::string, Ref<Mesh>> GetMeshes() { return m_Meshes; }
        Ref<Material> GetMeshMaterial(const std::string name) { return m_Materials[m_MeshMaterials[name]]; }

        static Ref<Model> Create(Ref<Mesh> mesh, Ref<Material> material);
        static Ref<Model> Create(const std::string path);
        static Ref<Model> Create(const Ref<ByteBuffer>& buffer);
    private:
        std::string m_Path;

        std::unordered_map<std::string, Ref<Mesh>> m_Meshes;
        std::unordered_map<std::string, Ref<Material>> m_Materials;
        std::unordered_map<std::string, std::string> m_MeshMaterials; //First is mesh, second is material

        void m_LoadModel(const aiScene* scene);
    };
} 