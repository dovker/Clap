#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in vec4 a_Tangent;

out VertexOutput
{
	vec3 FragPos;
    vec2 TexCoord;
	mat3 TBN;
} Output;

layout(std140) uniform Camera
{
	mat4 ViewProjection;
};

layout(std140) uniform ModelData
{
	mat4 Model;
};

void main()
{
    vec4 worldPos = Model * vec4(a_Position, 1.0);
    Output.FragPos = worldPos.xyz;
    Output.TexCoord = a_TexCoords;
    
    vec3 T = normalize(vec3(Model * vec4(a_Tangent.xyz, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(a_Normal, 0.0)));

    T = normalize(T - dot(T, N) * N);
    
    vec3 B = cross(N, T) * a_Tangent.w;

    Output.TBN = mat3(T, B, N);
    
    gl_Position = ViewProjection * worldPos;
}


#shader fragment
#version 330 core

in VertexOutput
{
    vec3 FragPos;
    vec2 TexCoord;
	mat3 TBN;
} Input;

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(Input.TBN[2], 1.0);
}

