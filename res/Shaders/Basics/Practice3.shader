#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
void main()
{
	gl_Position = vec4(aPos, 1.0);
}



#shader fragment
#version 330 core
out vec4 FragColor;
uniform vec3 updatedColor;
void main()
{
	FragColor = vec4(updatedColor, 1.0f);
	//FragColor = vec4(0.3f, 0.2f, 0.6f, 1.0f);
}