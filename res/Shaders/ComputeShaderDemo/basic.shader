#shader vertex
#version 430 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexture;
out vec2 textureCoord;

uniform mat4 mvp;

void main()
{

	gl_Position = mvp * vec4(aPos, 1.0f);
	textureCoord = aTexture;
}




#shader fragment
#version 430 core
in vec2 textureCoord;
out vec4 FragColor;



void main()
{
	FragColor = vec4(0.8f, 0.5f, 0.7f, 1.0f);
	//FragColor = mix(texture(hanon, textureCoord), texture(container, textureCoord), 0.2f);
}