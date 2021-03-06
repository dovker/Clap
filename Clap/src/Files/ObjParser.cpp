#include "ObjParser.h"

namespace Clap
{
    struct MeshVertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normals;
        glm::vec3 Tangents;
    };

    Ref<Mesh> ObjParser::Parse(const std::string& filename, bool precalculateTangents) //Keeps space for tangents regardless. If you want to import in some obscure manner, write a parser yourself, I am too lazy at this instance.
    {
        std::fstream file(filename);
        CLAP_ASSERT(file, "FSTREAM ERROR, FAILED READING THE FILE");
        
        struct IndexData
        {
            uint32_t vertex;
            uint32_t texCoord;
            uint32_t normal;
        };

        std::string tmp; //Temporary string

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;

        std::vector<uint32_t> indices;
        std::vector<MeshVertex> vertices;

        while(std::getline(file, tmp))
        {
            std::stringstream line(tmp);
            std::string header;
            std::getline(line, header, ' ');
            switch (header[0])
            {
            case 'v':
                switch(header[1])
                {
                    case 'n': 
                    {
                        float x, y, z;
                        line >> x >> y >> z;
                        normals.push_back( {x, y, z});
                    } break;
                    case 't': 
                    {
                        float u, v;
                        line >> u >> v;
                        texCoords.push_back( {u, v});
                    } break;
                    default:
                    {
                        float x, y, z;
                        line >> x >> y >> z;
                        positions.push_back({ x, y, z });
                    } break;
                }
            break;
            case 'f':
                //Gathering data
                std::string temp;
                IndexData currentFace[3];
                bool generateNormal = false;
                bool hasTexCoords = false;
                for(int i = 0; i < 3; i++)
                {
                    std::getline(line, temp, ' ');
                    std::stringstream subpart(temp);
                    
                    std::getline(subpart, temp, '/');
                    currentFace[i].vertex = std::atoi(temp.c_str()) - 1;

                    std::getline(subpart, temp, '/');
                    currentFace[i].texCoord = std::atoi(temp.c_str()) - 1;
                    hasTexCoords = currentFace[i].texCoord + 1 != 0;

                    std::getline(subpart, temp, ' ');
                    currentFace[i].normal = std::atoi(temp.c_str()) - 1;
                    generateNormal = currentFace[i].normal + 1 == 0;
                }
                glm::vec3 tangent;
                glm::vec3 bitangent;
                glm::vec3 precalcNormal;
                //Calculating tangent space
                if(precalculateTangents)
                {
                    glm::vec3 calcVerts[3];
                    glm::vec2 calcUVs[3];

                    CLAP_ASSERT(hasTexCoords, "Model has no texture coordinates. Cannot generate tangents");
                    for(int i = 0; i < 3; i++)
                    {
                        calcVerts[i] = positions[currentFace[i].vertex];
                        calcUVs[i] = texCoords[currentFace[i].texCoord];
                    }

                    glm::vec3 edge1 = calcVerts[1] - calcVerts[0];
                    glm::vec3 edge2 = calcVerts[2] - calcVerts[0];

                    glm::vec2 deltaUV1 = calcUVs[1] - calcUVs[0];
                    glm::vec2 deltaUV2 = calcUVs[2] - calcUVs[0];


                    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x); //summate these when doing actuall calcualtions
                    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

                    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                }
                if(generateNormal)
                {
                    glm::vec3 A = positions[currentFace[1].vertex] - positions[currentFace[0].vertex];
                    glm::vec3 B = positions[currentFace[2].vertex] - positions[currentFace[0].vertex];
                    precalcNormal = glm::cross(A, B);
                }
                for(int i = 0; i < 3; i++)
                {
                    indices.push_back(indices.size());
                    MeshVertex v;

                    v.Position = positions[currentFace[i].vertex];

                    if(hasTexCoords)
                        v.TexCoords = texCoords[currentFace[i].texCoord];

                    if(generateNormal)
                    {
                        v.Normals = precalcNormal;
                    }
                    else
                    {
                        v.Normals = normals[currentFace[i].normal];
                    }

                    if(precalculateTangents)
                    {
                        glm::vec3 normal = normals[currentFace[i].normal];
                        glm::vec3 orthogonalizedTangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));

                        //float w = (glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f) ? -1.0f : 1.0f;

                        v.Tangents = orthogonalizedTangent;
                    }
                    vertices.push_back(v);
                }
            break;
            }
        }
        return Mesh::Create(std::vector<float>((float*)vertices.data(), (float*)vertices.data() + vertices.size() * 11), indices, true);
    }
    Ref<Mesh> ObjParser::ParseString(const std::string& string)
    {
        return nullptr;
    }
}