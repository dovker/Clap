#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4 aWeights;

out VertexOutput
{
	vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
	mat3 TBN;
} Output;

layout(std140) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
};

void main()
{
    vec4 worldPos = Model * vec4(aPos, 1.0);
    Output.FragPos = worldPos.xyz;
    Output.TexCoord = aTexCoord;
    Output.Normal = aNormal;
    
    vec3 T = normalize(vec3(Model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(Model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(aNormal, 0.0)));
    
    Output.TBN = mat3(T, B, N);
    
    gl_Position = ViewProjection * worldPos;
}


#shader fragment
#version 450 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gEmissive;
layout (location = 4) out vec4 gMetallicRoughAOMat;

in VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
	mat3 TBN;
} Input;

layout(std140) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
};

layout(std140) uniform MaterialUniform
{
    vec4 Albedo;
    float Emissive;
    float Metallic;
    float Roughness;
};

layout (binding = 0) uniform sampler2D uAlbedoTex;
layout (binding = 1) uniform sampler2D uNormalTex;
layout (binding = 2) uniform sampler2D uMetallicTex;
layout (binding = 3) uniform sampler2D uRoughnessTex;
layout (binding = 4) uniform sampler2D uAOTex;
layout (binding = 5) uniform sampler2D uEmissiveTex;

void main()
{
    gPosition = vec4(Input.FragPos, 1.0);

    vec3 normal = texture(uNormalTex, Input.TexCoord).rgb;
    if(normal == vec3(1.0))
        normal = vec3(0.0, 0.0, 1.0); //Default normal
    normal = normal * 2.0 - 1.0;   
    vec3 sampledNormal = normalize(Input.TBN * normal);
    gNormal = vec4(sampledNormal, 1.0);

    vec4 albedo = texture(uAlbedoTex, Input.TexCoord);
    gAlbedo = vec4(albedo * Albedo);
    
    vec3 emissive = texture(uEmissiveTex, Input.TexCoord).rgb;
    gEmissive = vec4(emissive * Emissive, 1.0);

    float metallic = texture(uMetallicTex, Input.TexCoord).r;
    float roughness = texture(uRoughnessTex, Input.TexCoord).r;
    float ao = texture(uAOTex, Input.TexCoord).r;

    gMetallicRoughAOMat = vec4(metallic * Metallic, roughness * Roughness, ao, 1.0);
}
