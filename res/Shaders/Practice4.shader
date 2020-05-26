#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 i_color;
layout(Location = 2) in vec2 i_textureCood;
out vec4 color;
out vec2 textureCood;
void main() 
{
	gl_Position = vec4(aPos, 1.0f);
	color = vec4(i_color, 1.0f);
	textureCood = i_textureCood;
}

#shader fragment
#version 330 core
in vec4 color;
in vec2 textureCood;
out vec4 FragColor;
void main() 
{
	// TODO: implement texture color
	//FragColor = vec4(0.4, 0.6, 0.8, 1.0);
	FragColor = color;
}
