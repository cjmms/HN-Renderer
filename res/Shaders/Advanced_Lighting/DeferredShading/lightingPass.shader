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

    float Linear;
    float Quadratic;
    float Radius;       // light volume
};

// 32 light source
const int N_LIGHTS = 32;

uniform Light lights[N_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D AlbedoSpec;

uniform vec3 viewPos;


void main()
{
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoord).rgb;
    vec3 Normal = texture(gNormal, TexCoord).rgb;
    vec3 Albedo = texture(AlbedoSpec, TexCoord).rgb;
    float Specular = texture(AlbedoSpec, TexCoord).a;

    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.01; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < N_LIGHTS; ++i)
    {
        // distance from fragment to light source
        float distance = length(lights[i].Position - FragPos);

        if (distance < lights[i].Radius)
        {
            // diffuse
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;

            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].Color * spec * Specular;

            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;


            lighting += diffuse;
            lighting += specular;
        }
    }


    // tone mapping with exposure
    float exposure = 1.0f;
    vec3 mappedColor = vec3(1.0) - exp(-lighting * exposure);

    float gamma = 2.2f;

    mappedColor = pow(mappedColor, vec3(1.0f / gamma));

    FragColor = vec4(mappedColor, 1.0f);
}