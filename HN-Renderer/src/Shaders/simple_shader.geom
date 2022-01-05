#version 450

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;


//layout(location = 0) out vec3 fragPosWorld;
//layout(location = 1) out vec3 fragNormalWorld;


layout(location = 0) in vec3 worldNormal[];


layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec4 lightColor;
} ubo;


void main(void)
{
	float normalLength = 0.5;
	float width = 0.1;
	for(int i=0; i<gl_in.length(); i++)
	{
		vec3 pos = gl_in[i].gl_Position.xyz;
		vec3 normal = worldNormal[i].xyz;

		vec4 viewPos = ubo.view *  vec4(pos, 1.0);

		gl_Position = ubo.projection * (viewPos - vec4(width / 2, 0, 0, 0));	// happens inside view space
		EmitVertex();

		gl_Position = ubo.projection * (viewPos + vec4(width / 2, 0, 0, 0));
		EmitVertex();

		//gl_Position = ubo.projection * ubo.view *  vec4(pos, 1.0);
		//EmitVertex();

		gl_Position = ubo.projection * ubo.view *  vec4(pos + normal * normalLength, 1.0);		
		EmitVertex();

		EndPrimitive();
	}



}
