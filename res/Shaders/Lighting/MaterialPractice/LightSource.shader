#vertex shader
#version 330 core
layout(Location = 0) in vec3 aPos;

uniform mat4 mvp;

void main() 
{
	gl_Position = mvp * vec4(aPos, 1.0f);
}



#fragment shader
#version 330 core

out vec4 FragColor;

uniform vec3 LightColor;

void main() 
{
	FragColor = vec4(LightColor, 1.0f);
}