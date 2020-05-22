#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
out vec4 vertexColor;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

#shader fragment
#version 330 core
in vec4 vertexColor;
out vec4 FragColor;
uniform vec4 uniColor;

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = uniColor;
}