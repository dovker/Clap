#shader vertex
#version 450 core
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
#version 450 core

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

layout (binding = 0) uniform sampler2D uTextures[16];

float threshold = 0.5;
float smoothness = 0.005;

void main()
{
    
    if (Input.Data.x <= 0.0)
    {
        FragColor = Input.Color;
    } 
    else
    {
        if(Input.Data.y < 1.0) //REGULAR FONT RENDERING
        {
            vec4 color = vec4(vec3(1.0), texture(uTextures[int(Input.Data.x)], Input.TexCoords).r) * Input.Color;

            if(color.a == 0.0) discard;

            FragColor = color;
        }
        else if(Input.Data.y < 2.0) // SDF FONT RENDERING
        {
            float value = texture(uTextures[int(Input.Data.x)], Input.TexCoords).r;

            vec4 color = vec4(vec3(1.0), smoothstep(threshold - smoothness, threshold + smoothness, value));

            if(color.a == 0.0) discard;

            FragColor = color;
        }
        else // SPRITEFONT RENDERING
        {
            vec4 color = texture(uTextures[int(Input.Data.x)], Input.TexCoords) * Input.Color;
            if (color.a <= 0.0)
            {
                discard;
            }

            FragColor = color;
        }
        
    }
}
