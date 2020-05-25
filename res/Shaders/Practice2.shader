#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 icolor;
out vec3 ocolor;
void main()
{
	gl_Position = vec4(aPos, 1.0);
	//ocolor = icolor;		// Transparency always 1.0f
	ocolor = aPos;
}




#shader fragment
#version 330 core
in vec3 icolor;
out vec4 FragColor;
void main()
{
	FragColor = vec4(icolor, 1.0f);
	//FragColor = vec4(0.3f, 0.2f, 0.6f, 1.0f);
}