#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;



void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;

}





#shader fragment
#version 330 core

struct Light {
	vec3 Position;
	vec3 Color;
};


in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform Light lights[4];
uniform sampler2D diffuseMap;
uniform vec3 viewPos;


vec3 calculateLighting(vec3 normal, int i, vec3 color)
{
	// diffuse
	vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 result = lights[i].Color * diff * color;
	// attenuation (use quadratic as we have gamma correction)
	float distance = length(fs_in.FragPos - lights[i].Position);
	result *= 1.0 / (distance * distance);
	return result;
}


void main()
{
	vec3 color = texture(diffuseMap, fs_in.TexCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.15 * color;
	vec3 lighting = vec3(1.0f);

	for (int i = 0; i < 4; ++i)
	{
		lighting += calculateLighting(normal, i, color);
	}

	FragColor = vec4(texture(diffuseMap, fs_in.TexCoord).rgb, 1.0f);
	//FragColor = vec4(ambient + lighting, 1.0f);
}