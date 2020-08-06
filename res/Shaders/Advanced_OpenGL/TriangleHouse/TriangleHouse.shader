#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;

uniform mat4 mvp;


void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	gl_PointSize = 10.0f;
}




#shader geometry
#version 330 core
layout(points) in;
layout(points, max_vertices = 5) out;

void main() 
{
	gl_Position = gl_in[0].gl_Position;
	gl_PointSize = gl_in[0].gl_PointSize;
	EmitVertex();
	EndPrimitive();
}




#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.4f, 0.2f, 0.8f, 1.0f);
}
