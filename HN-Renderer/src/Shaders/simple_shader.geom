#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


layout(location = 0) in vec3 outNormal[];


layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec4 lightColor;
} ubo;


layout(push_constant) uniform Push 
{
	mat4 modelMat;
} push;


void main(void)
{
	float normalLength = 0.5;
	float width = 0.1;
	for(int i=0; i<gl_in.length(); i++)
	{
		vec3 worldPos = vec3(push.modelMat * gl_in[i].gl_Position);
		vec3 normal = normalize(mat3(push.modelMat) * outNormal[i]);


		vec4 viewPos = ubo.view * vec4(worldPos, 1.0);

		gl_Position = ubo.projection * (viewPos - vec4(width / 2, 0, 0, 0));	// happens inside view space
		EmitVertex();

		gl_Position = ubo.projection * (viewPos + vec4(width / 2, 0, 0, 0));
		EmitVertex();

		gl_Position = ubo.projection * ubo.view *  vec4(worldPos + normal * normalLength, 1.0);		
		EmitVertex();

		EndPrimitive();
	}
}