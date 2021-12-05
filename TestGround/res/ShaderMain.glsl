#shader vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

struct VertexOutput
{
	vec3 Normal;
};
out VertexOutput Output;

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
    
    Output.Normal = normalize(a_Normal);
    
    gl_Position = ViewProjection * worldPos;
}


#shader fragment
#version 330 core

struct VertexOutput
{
	vec3 Normal;
};

in VertexOutput Input;

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0);
}

