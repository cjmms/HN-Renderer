#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}




#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

struct Light
{
    vec3 Position;
    vec3 Color;
};

// 32 light source
const int N_LIGHTS = 32;

uniform Light lights[N_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D diffuseMap;

uniform vec3 viewPos;


void main()
{
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(diffuseMap, TexCoord).rgb;

    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    for (int i = 0; i < N_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        lighting += diffuse;
    }


    // tone mapping with exposure
    float exposure = 1.0f;
    vec3 mappedColor = vec3(1.0) - exp(-lighting * exposure);

    float gamma = 2.2f;

    mappedColor = pow(mappedColor, vec3(1.0f / gamma));

    FragColor = vec4(mappedColor, 1.0f);


    //FragColor = vec4(lighting, 1.0);
}