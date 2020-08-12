#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out vec3 normal;
out vec2 textureCoord;
out vec3 FragPos;

uniform mat4 mvp;
uniform mat4 view;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);

	normal =  aNormal;
	textureCoord = aTextureCoord;
	FragPos = aPos;
}



#shader fragment
#version 330 core

in vec3 normal;
in vec2 textureCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture_wood;

void main()
{
	FragColor = texture(texture_wood, textureCoord);
}