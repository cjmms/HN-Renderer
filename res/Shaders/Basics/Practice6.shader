#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexture;
out vec2 textureCoord;

uniform mat4 mvp;

void main() 
{

	gl_Position = mvp * vec4(aPos, 1.0f);
	textureCoord = aTexture;
}




#shader fragment
#version 330 core
in vec2 textureCoord;
out vec4 FragColor;

uniform sampler2D hanon;
uniform sampler2D container;

void main() 
{
	//FragColor = vec4(0.8f, 0.5f, 0.7f, 1.0f);
	FragColor = mix(texture(hanon, textureCoord), texture(container, textureCoord), 0.2f);
}