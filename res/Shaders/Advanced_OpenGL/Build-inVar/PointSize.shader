#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;

uniform mat4 mvp;


void main() {
	gl_Position = mvp * vec4(aPos, 1.0f);
	gl_PointSize = gl_Position.z;
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.3f, 1.0f);
}