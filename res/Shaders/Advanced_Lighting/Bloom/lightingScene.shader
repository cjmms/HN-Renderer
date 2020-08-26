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

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;


out vec4 FragColor;

uniform sampler2D diffuseMap;

void main()
{
	FragColor = vec4(texture(diffuseMap, fs_in.TexCoord).rgb, 1.0f);

}