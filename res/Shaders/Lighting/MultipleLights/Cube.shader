#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexture;

uniform mat4 mvp;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 Texture;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Texture = aTexture;
}
