#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 i_color;
layout(Location = 2) in vec3 i_textureCood;
out vec4 color;
out vec4 textureCood;
void main() 
{
	gl_Position = vec4(aPos, 1.0f);
	color = vec4(i_color, 1.0f);
	textureCood = vec4(i_textureCood, 1.0f);
}

#shader fragment
#version 330 core
in vec4 color;
in vec4 textureCood;
out FragColor;
void main() 
{
	// TODO: implement texture color
	FragColor = color;
}
