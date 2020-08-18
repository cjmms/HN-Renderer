#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	// model space -> world space
	gl_Position = model * vec4(aPos, 1.0f);
}



#shader geometry
#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 lightViews[6];
uniform mat4 lightProjection;

out vec4 FragPos;

// take one triangle as input, output 6 triangles
void main()
{
	// iterate 6 cubemap faces through build-in var: gl_Layer
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		// iterate 3 triangle vertices
		for (int i = 0; i < 3; ++i)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = lightProjection * lightViews[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}



#shader fragment
#version 330 core

uniform vec3 lightPos;
uniform float farPlane;

in vec4 FragPos;


// no output color
// setup depth value manunally through gl_FragDepth
void main()
{
	float light2FragDistance = length(lightPos - FragPos.xyz);

	// map to [0, 1]
	float depth = light2FragDistance / farPlane;

	// write to depth buffer
	gl_FragDepth = depth;
}




