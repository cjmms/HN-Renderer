#version 450

layout (vertices = 3) out;

layout(location = 0) in vec3 worldNormal[];

layout(location = 0) out vec3 outNormal[];


layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec4 lightColor;
} ubo;


void main()
{
	float tessL = 5;

	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = tessL;
		gl_TessLevelOuter[0] = tessL;
		gl_TessLevelOuter[1] = tessL;
		gl_TessLevelOuter[2] = tessL;		
	}

	gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position;
	outNormal[gl_InvocationID] = worldNormal[gl_InvocationID];
}