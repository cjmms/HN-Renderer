#version 450

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


//layout(location = 0) out vec3 fragPosWorld;
//layout(location = 1) out vec3 fragNormalWorld;


layout(location = 0) in vec3 worldNormal[];


layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projViewMat;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec4 lightColor;
} ubo;


void main(void)
{
	float normalLength = 0.02;
	for(int i=0; i<gl_in.length(); i++)
	{
		vec3 pos = gl_in[i].gl_Position.xyz;
		vec3 normal = worldNormal[i].xyz;

		gl_Position = ubo.projViewMat *  vec4(pos, 1.0);
		EmitVertex();

		gl_Position = ubo.projViewMat * ( vec4(pos + normal * normalLength, 1.0));		
		EmitVertex();

		EndPrimitive();
	}



}
