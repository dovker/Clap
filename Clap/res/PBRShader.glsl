#shader vertex
#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out VertexOutput
{
    vec2 TexCoord;
} Output;

void main()
{
    Output.TexCoord = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 450 core

in VertexOutput
{
    vec2 TexCoord;
} Input;

layout(std140, binding = 0) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
    vec3 CameraPos;
    vec3 CameraDir;
};


layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gAlbedo;
layout (binding = 3) uniform sampler2D gEmissive;
layout (binding = 4) uniform sampler2D gMetallicRoughAOMat;
layout (binding = 5) uniform sampler2D gDepth;

layout (location = 0) out vec4 FragColor;

float near = 0.01;
float far = 10000.0;

float LinearizeDepth(float depth)
{
    depth = depth * 2.0 - 1.0;
    
    return (2.0 * near * far) / (far + near - depth * (far - near));	
}

void main()
{
    vec3 Position = texture(gPosition, Input.TexCoord).rgb;
    vec3 Normal = texture(gNormal, Input.TexCoord).rgb;

    vec4 Albedo = texture(gAlbedo, Input.TexCoord);

    vec3 Emissive = texture(gEmissive, Input.TexCoord).rgb;

    vec3 gMetallicRoughAOMat = texture(gMetallicRoughAOMat, Input.TexCoord).rgb;
    float Metallic = gMetallicRoughAOMat.r;
    float Roughness = gMetallicRoughAOMat.g;
    float AO = gMetallicRoughAOMat.b;

    float Depth = texture(gDepth, Input.TexCoord).r;
    
    FragColor = vec4(vec3(LinearizeDepth(Depth) / far), 1.0);
}
