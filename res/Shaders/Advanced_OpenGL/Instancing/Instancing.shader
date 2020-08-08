#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec3 aColor;

out vec3 color;

uniform vec2 offsets[100];

void main()
{
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(aPos + offset, 0.0f, 1.0f);
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