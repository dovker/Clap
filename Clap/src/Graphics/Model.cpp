#include "pch.h"

#include "Model.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Clap
{
    Ref<Texture2D> LoadAssimpTextureFromPath(aiString path, std::string filePath)
    {
        std::string fixedPath =  std::string(path.C_Str());
        std::string modelPath = filePath.substr(0, filePath.find_last_of('/'));
        for (int i = 0; i < fixedPath.length(); ++i) {
            if (fixedPath[i] == '\\')
                fixedPath[i] = '/';
        }
        return Texture2D::Create(modelPath + "/" + fixedPath);
    }
    Ref<Texture2D> LoadAssimpTexture(aiTextureType type, aiMaterial* aiMaterial, const aiScene* scene, std::string filePath)
    {
        aiString texturePath;
        aiReturn result = aiMaterial->GetTexture(type, 0, &texturePath);

        if (result == AI_SUCCESS) {
            if (texturePath.data[0] == '*') {
                // Embedded texture found
                unsigned int textureIndex = std::stoi(std::string(texturePath.C_Str()).substr(1)); // Convert char to int
                aiTexture* aiTexture = scene->mTextures[textureIndex]; //TODO: Optimize this so that models have a list of textures
                Ref<Texture2D> texture;
                if(aiTexture->mHeight == 0)
                {
                    Ref<ByteBuffer> rawImage = ByteBuffer::Create(aiTexture->pcData, aiTexture->mWidth);
                    texture = Texture2D::Create(rawImage);
                } else
                {
                    texture = Texture2D::Create(aiTexture->mWidth, aiTexture->mHeight, {TextureFormat::RGBA8, FilterType::NEAREST, FilterType::NEAREST});
                    texture->SetData(aiTexture->pcData, aiTexture->mWidth * aiTexture->mHeight * 4);
                }
                return texture;
            } else {
                // Load texture from file
                return LoadAssimpTextureFromPath(texturePath, filePath);
            }
        }
        return nullptr;
    }

    void Model::m_LoadModel(const aiScene* scene)
    {
        CLAP_ASSERT(scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode, "FAILED TO LOAD MODEL");

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh* aiMesh = scene->mMeshes[i];

            // Extract mesh data: positions, normals, texCoords, indices
            //TODO: Optimize this to use a single VertexArray

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            vertices.reserve(aiMesh->mNumVertices);
            
            indices.reserve(aiMesh->mNumFaces * 3);

            for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
                glm::vec2 uvCoord;

                if (aiMesh->mTextureCoords[0]) {
                    uvCoord.x = aiMesh->mTextureCoords[0][i].x;
                    uvCoord.y = aiMesh->mTextureCoords[0][i].y;
                }
                else {
                    uvCoord.x = 0.0f;
                    uvCoord.y = 0.0f;
                }

                Vertex vertex;

                vertex.Position.x  = (aiMesh->mVertices[i].x);
                vertex.Position.y  = (aiMesh->mVertices[i].y);
                vertex.Position.z  = (aiMesh->mVertices[i].z);
                vertex.TexCoord.x  = (uvCoord.x);
                vertex.TexCoord.y  = (uvCoord.y);
                vertex.Normal.x    = (aiMesh->mNormals[i].x);
                vertex.Normal.y    = (aiMesh->mNormals[i].y);
                vertex.Normal.z    = (aiMesh->mNormals[i].z);
                vertex.Tangent.x   = (aiMesh->mTangents[i].x);
                vertex.Tangent.y   = (aiMesh->mTangents[i].y);
                vertex.Tangent.z   = (aiMesh->mTangents[i].z);

                vertices.push_back(vertex);
            }

            for (uint32_t i = 0; i < aiMesh->mNumFaces; ++i) {
                aiFace face = aiMesh->mFaces[i];
                for (uint32_t j = 0; j < face.mNumIndices; ++j) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            // Extract PBR material properties
            PBRMaterialUniform materialData;
            Ref<Material> material = Material::Create(materialData);
            aiMaterial* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

            aiColor3D baseColor;
            float opacity = 1.0f;
            aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor);
            aiMaterial->Get(AI_MATKEY_OPACITY, opacity);
            materialData.Albedo = glm::vec4(baseColor.r, baseColor.g, baseColor.b, opacity);

            float metallic;
            aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metallic);
            materialData.Metallic = metallic;

            float roughness;
            aiMaterial->Get(AI_MATKEY_SHININESS, roughness);
            materialData.Roughness = 1.0f - roughness; 

            material->SetData(&materialData);

            Ref<Texture2D> currentTexture;

            currentTexture = LoadAssimpTexture(aiTextureType_DIFFUSE, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("Albedo", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_BASE_COLOR, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("Albedo", currentTexture);
            }

            currentTexture = LoadAssimpTexture(aiTextureType_NORMALS, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("Normals", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_NORMAL_CAMERA, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("Normals", currentTexture);
            }

            currentTexture = LoadAssimpTexture(aiTextureType_SPECULAR, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("Metallic", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_METALNESS, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("Metallic", currentTexture);
            }

            currentTexture = LoadAssimpTexture(aiTextureType_SHININESS, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("Roughness", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_DIFFUSE_ROUGHNESS, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("Roughness", currentTexture);
            }

            currentTexture = LoadAssimpTexture(aiTextureType_AMBIENT, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("AO", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_AMBIENT_OCCLUSION, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("AO", currentTexture);
            }
            
            currentTexture = LoadAssimpTexture(aiTextureType_EMISSIVE, aiMaterial, scene, m_Path);
            if(currentTexture)
                material->SetTexture("Emission", currentTexture);
            else 
            {
                currentTexture = LoadAssimpTexture(aiTextureType_EMISSION_COLOR, aiMaterial, scene, m_Path);
                if(currentTexture)
                    material->SetTexture("Emission", currentTexture);
            }

            aiString materialName;
            aiMaterial->Get(AI_MATKEY_NAME, materialName);

            std::string meshName = aiMesh->mName.C_Str();
            Ref<Mesh> mesh = Mesh::Create(vertices, indices);

            m_Materials[materialName.C_Str()] = material;
            m_Meshes[meshName] = mesh;
            m_MeshMaterials[meshName] = materialName.C_Str();
        }
    }


    Model::Model(Ref<Mesh> mesh, Ref<Material> material)
    {
        m_Meshes["Mesh"] = mesh;
        m_Materials["Material"] = material;
        m_MeshMaterials["Mesh"] = "Material";
    }

    Model::Model(const std::string path)
        : m_Path(path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
                                                aiProcess_Triangulate |
                                                //aiProcess_FlipUVs | /// DON'T NEED TO
                                                aiProcess_GenNormals |
                                                aiProcess_CalcTangentSpace);
        m_LoadModel(scene);
    }

    Model::Model(const Ref<ByteBuffer>& buffer)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory((void*)buffer->GetData().data(), buffer->GetData().size(),
                                                aiProcess_Triangulate |
                                                //aiProcess_FlipUVs |
                                                aiProcess_GenNormals |
                                                aiProcess_CalcTangentSpace);
        m_LoadModel(scene);
    }

    Ref<Model> Model::Create(Ref<Mesh> mesh, Ref<Material> material)
    {
        return CreateRef<Model>(mesh, material);
    }

    Ref<Model> Model::Create(const std::string path)
    {
        return CreateRef<Model>(path);
    }

    Ref<Model> Model::Create(const Ref<ByteBuffer>& buffer)
    {
        return CreateRef<Model>(buffer);
    }


    // void Model::Draw(const Ref<UniformBuffer>& uniformBuffer)
    // {
    //     for(auto mesh : m_Meshes)
    //     {
    //         Ref<Material> material = m_Materials[mesh.first];
    //         uniformBuffer->SetData(material->GetData());

    //     }
    // }
}