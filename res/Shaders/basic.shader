#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 colorPos;
out vec4 vertexColor;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertexColor = vec4(colorPos, 1.0f);
}

#shader fragment
#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vertexColor;
}