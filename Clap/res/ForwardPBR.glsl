#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out VertexOutput
{
	vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
	mat3 TBN;
} Output;

layout(std140, binding = 0) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
    vec3 CameraPos;
    vec3 CameraDir;
};

void main()
{
    vec4 worldPos = Model * vec4(aPos, 1.0);
    Output.FragPos = worldPos.xyz;
    Output.TexCoord = aTexCoord;
    Output.Normal = aNormal;
    
    vec3 T = normalize(vec3(Model * vec4(aTangent, 0.0)));
    vec3 bitangent = normalize(cross(aNormal, aTangent));
    vec3 B = normalize(vec3(Model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(aNormal, 0.0)));
    
    Output.TBN = mat3(T, B, N);
    
    gl_Position = ViewProjection * worldPos;
}


#shader fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

in VertexOutput
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
	mat3 TBN;
} Input;

layout(std140, binding = 0) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
    vec3 CameraPos;
    vec3 CameraDir;
};

layout(std140, binding = 1) uniform MaterialUniform
{
    vec4 Albedo;
    float Emissive;
    float Metallic;
    float Roughness;
    float Translucent;
};

layout (binding = 0) uniform sampler2D uAlbedoTex;
layout (binding = 1) uniform sampler2D uNormalTex;
layout (binding = 2) uniform sampler2D uMetallicTex;
layout (binding = 3) uniform sampler2D uRoughnessTex;
layout (binding = 4) uniform sampler2D uAOTex;
layout (binding = 5) uniform sampler2D uEmissiveTex;
layout (binding = 6) uniform sampler2D uBackFaceDepth;

float near = 0.01;
float far = 10000.0;

float LinearizeDepth(float depth)
{
    depth = depth * 2.0 - 1.0;
    
    return (2.0 * near * far) / (far + near - depth * (far - near));	
}

void main()
{
    vec3 normal = texture(uNormalTex, Input.TexCoord).rgb;
    vec3 sampledNormal;
    if(normal == vec3(1.0))
        sampledNormal = Input.TBN[2];
    else
    {
        normal = normal * 2.0 - 1.0;   
        sampledNormal = normalize(Input.TBN * normal);
    }
    normal = sampledNormal;

    vec4 albedo = texture(uAlbedoTex, Input.TexCoord) * Albedo;
    
    vec3 emissive = texture(uEmissiveTex, Input.TexCoord).rgb * Emissive;

    float metallic = texture(uMetallicTex, Input.TexCoord).r;
    float roughness = texture(uRoughnessTex, Input.TexCoord).r;
    float ao = texture(uAOTex, Input.TexCoord).r;

    vec4 ClipCoord = ViewProjection * vec4(Input.FragPos, 1.0);
    vec3 NDC = ClipCoord.xyz/ClipCoord.w;
    vec2 UV = (NDC.xy + 1.0) / 2;

    float depth = texture(uBackFaceDepth, UV).r;
    float backDist = LinearizeDepth(depth);
    float frontDist = LinearizeDepth(gl_FragCoord.z);

    FragColor = vec4(vec3(1.0 - (abs(frontDist - backDist) / 100.0)), 1.0);
    //FragColor = vec4(UV, 0.0, 1.0);
}
