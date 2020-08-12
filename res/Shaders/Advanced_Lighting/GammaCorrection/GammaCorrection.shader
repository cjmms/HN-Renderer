#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT {
	vec3 FragPos;
	vec3 normal;
	vec2 textureCoord;
} vs_out;

uniform mat4 view;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);

	vs_out.FragPos = aPos;
	vs_out.normal = aNormal;
	vs_out.textureCoord = aTextureCoord;
}





#shader fragment
#version 330 core


in VS_OUT {
	vec3 FragPos;
	vec3 normal;
	vec2 textureCoord;
} fs_in;


out vec4 FragColor;

uniform sampler2D texture_wood;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform bool change;


void main()
{
	FragColor = texture(texture_wood, fs_in.textureCoord);
}