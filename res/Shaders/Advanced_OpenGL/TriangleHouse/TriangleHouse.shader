#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;


void main()
{
	gl_Position =  vec4(aPos, 1.0f);
}




#shader geometry
#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

void build_house(vec4 position)
{
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);	// 1:bottom-left
	EmitVertex();
	
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
	EmitVertex();

	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:top-left
	EmitVertex();

	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:top-right
	EmitVertex();

	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:top
	EmitVertex();

	EndPrimitive();
}


void main() 
{
	build_house(gl_in[0].gl_Position);
}




#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.4f, 0.2f, 0.8f, 1.0f);
}
