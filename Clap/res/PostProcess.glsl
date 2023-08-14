#shader vertex
#version 450 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gColor_0;

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
    vec4 Sample = texture(gColor_0, TexCoords);
    vec3 Albedo = Sample.rgb;
    vec3 Color = vec3(Albedo);
    
    FragColor = vec4(Color, Sample.a);
    //FragColor = vec4(Albedo, Sample.a);
}
