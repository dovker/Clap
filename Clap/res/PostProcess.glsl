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

out vec4 FragColor;

in VertexOutput
{
    vec2 TexCoord;
} Input;

layout (binding = 0) uniform sampler2D uColor;

layout(std140, binding = 0) uniform PostProcess
{
	float u_Exposure;        
    float u_WhiteBallance;
    float u_Tint;
    float u_Contrast;
    float u_Brightness;
    float u_Saturation;
    float u_Gamma;
};


void main()
{
    vec3 Sample = texture(uColor, Input.TexCoord).rgb;
    
    FragColor = vec4(Sample, 1.0);
}
