#version 450

layout(triangles, equal_spacing, cw) in;

layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	int tessellationLevel;
	vec4 lightColor;
	float height;
    float width;
} ubo;


layout(location = 0) in vec3 inNormal[];
layout(location = 0) out vec3 outNormal;


vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 


void main()
{
	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position); 
	outNormal = gl_TessCoord.x * inNormal[0] + gl_TessCoord.y * inNormal[1] + gl_TessCoord.z * inNormal[2]; 			
}