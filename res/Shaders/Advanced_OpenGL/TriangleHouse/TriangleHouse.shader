#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aColor;

out VS_OUT
{
	vec3 color;
} vs_out;


void main()
{
	gl_Position =  vec4(aPos, 1.0f);
	vs_out.color = aColor;
}




#shader geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT
{
	vec3 color;
} gs_in[];

out vec3 fcolor;

void build_house(vec4 position)
{
	fcolor = gs_in[0].color;

	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);	// 1:bottom-left
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:top-left
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:top-right
	EmitVertex();

	fcolor = vec3(1.0f, 1.0f, 1.0f);
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

in vec3 fcolor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(fcolor, 1.0f);
}
