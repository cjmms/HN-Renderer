#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
void main() 
{
	gl_Position = vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core
out vec4 FragColor;
void main() 
{
	FragColor = vec4(0.6f, 0.2f, 0.9f, 1.0f);
}