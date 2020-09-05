#shader vertex
#version 330 core

// This shader is for the geometry pass of deferred shading

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;


void main()
{
	vs_out.Position = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;

	gl_Position = projection * view * vec4(vs_out.Position, 1.0f);
}





#shader fragment
#version 330 core

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;


out vec4 FragColor;

uniform sampler2D diffuseMap;

void main()
{
	FragColor = vec4(0.2, 0.6, 0.5, 1.0f);
}