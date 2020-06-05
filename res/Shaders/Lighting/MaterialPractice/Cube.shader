#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 model;

out vec3 Normal;

void main() 
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	Normal = aNormal;
}




#shader fragment
#version 330 core

in vec3 Normal;

out vec4 FragColor;

void main() 
{
	FragColor = vec4(0.6);
}






