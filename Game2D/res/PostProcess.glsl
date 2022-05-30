#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gColor_0;


layout(std140) uniform PostProcess
{
	vec3 u_Ambient;
    float u_Gamma;
    float u_Exposure;
    float u_Brightness;
    float u_Contrast;
    float u_Saturation;
    float u_BlackPoint;
    float u_WhitePoint;
};


void main()
{
    vec4 Sample = texture(gColor_0, TexCoords);
    vec3 Albedo = Sample.rgb;
    vec3 Color = vec3(Albedo * u_Ambient);
    
    vec3 mapped = vec3(1.0) - exp(-Color * u_Exposure);
      // gamma correction
    mapped = pow(mapped, vec3(1.0 / u_Gamma));
    
    //Brightness pass
    mapped += u_Brightness;
    
    //Contrast Pass
    mapped = ((mapped - 0.5f) * max(u_Contrast, 0)) + 0.5f;
    
    //y = ax + b
    //White Point
    //Black Point
    mapped = mapped * u_WhitePoint * (1 - u_BlackPoint) + u_BlackPoint;
    

    //Saturation Pass
    vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 ); //TODO: ADD SLIDER
    float intensity = dot(mapped, luminance);
    mapped = mix(vec3(intensity), mapped, u_Saturation);
    
    FragColor = vec4(mapped, Sample.a);
}
