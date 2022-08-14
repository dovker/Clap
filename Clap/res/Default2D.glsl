#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aData;


out VertexOutput
{
	vec4 Color;
    vec3 FragPos;  
    vec2 TexCoords; 
    vec4 Data; 
} Output;

layout(std140) uniform Camera
{
	mat4  ViewProjection;
};

void main()
{
    Output.TexCoords = aTexCoords;
    Output.FragPos = aPos;
    
    Output.Color = aColor;
    Output.Data = aData;
    gl_Position = ViewProjection * vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

in VertexOutput
{
	vec4 Color;
    vec3 FragPos;  
    vec2 TexCoords; 
    vec4 Data; 
} Input;

layout(std140) uniform Camera
{
	mat4 ViewProjection;
};

layout(location = 0) out vec4 FragColor;

uniform sampler2D uTextures[16];

void main()
{
    
    if (Input.Data.x <= 0.0)
    {
        FragColor = Input.Color;
    } 
    else
    {
        vec4 color = texture(uTextures[int(Input.Data.x)], Input.TexCoords) * Input.Color;
        if (color.a <= 0.0)
        {
            discard;
        }

        FragColor = color;
    }
    //FragColor = vec4(1.0);
}
