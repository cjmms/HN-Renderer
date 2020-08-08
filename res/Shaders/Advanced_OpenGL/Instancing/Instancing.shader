#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec3 aColor;
layout(Location = 2) in vec2 offset;	// offset is passed by instanced array

out vec3 color;

void main()
{
	vec2 pos = aPos * (gl_InstanceID / 100.0f);
	gl_Position = vec4(pos + offset, 0.0f, 1.0f);
	color = aColor;
}



#shader fragment
#version 330 core

in vec3 color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0f);
}