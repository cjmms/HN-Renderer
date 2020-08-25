#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT{
	vec2 texCoord;
	vec3 FragPos;
	vec3 Normal;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.texCoord = aTex;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * -aNormal;

}




#shader fragment
#version 330 core

in VS_OUT
{
	vec2 texCoord;
	vec3 FragPos;
	vec3 Normal;
} fs_in;

struct Light
{
	vec3 Position;
	vec3 Color;
};


uniform Light lights[16];
uniform sampler2D diffuseMap;

uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	vec3 color = texture(diffuseMap, fs_in.texCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lighting = vec3(0.0f);

	vec3 ambient = 0.05 * color;

	for (int i = 0; i < 16; i++)
	{
		// diffuse
		vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);

		vec3 diffuse = lights[i].Color * color * max(dot(lightDir, normal), 0.0f);

		vec3 result = diffuse;

		// attenuation
		// using quadratic since gamma correction is applied
		float length = length(fs_in.FragPos - lights[i].Position);
		result *= 1.0 / (length * length);

		lighting += result;
	}

	//lighting = color;
	FragColor = vec4(ambient + lighting, 1.0f);
}