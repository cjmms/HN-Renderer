#pra#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;

uniform mat4 mvp;


void main() {
	gl_Position = mvp * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	if (gl_FragCoord.x < 600)
		FragColor = vec4(1.0f, 0.5f, 0.3f, 1.0f);
	else
		FragColor = vec4(0.6f, 0.3f, 0.9f, 1.0f);
}