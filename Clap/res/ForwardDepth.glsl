#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;


layout(std140) uniform CameraModelUniform
{
	mat4 ViewProjection;
    mat4 Model;
    vec3 CameraPos;
    vec3 CameraDir;
};

out float vertDepth;

void main()
{
    vec4 worldPos = Model * vec4(aPos, 1.0);
    vertDepth = worldPos.z;
    gl_Position = ViewProjection * worldPos;
}


#shader fragment
#version 450 core

in float vertDepth;

void main()
{
    //gl_FragDepth = vertDepth;
}
