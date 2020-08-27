#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}



#shader fragment
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D lightingScene;

void main()
{
	FragColor = vec4(texture(lightingScene, TexCoord).rgb, 1.0f);
}