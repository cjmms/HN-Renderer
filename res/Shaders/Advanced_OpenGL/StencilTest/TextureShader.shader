#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTextureCoord;

uniform mat4 mvp;

out vec2 TextureCoord;

void main() 
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	TextureCoord = aTextureCoord;
}


#shader fragment
#version 330 core

in vec2 TextureCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main() 
{
	FragColor = texture(texture0, TextureCoord);
}