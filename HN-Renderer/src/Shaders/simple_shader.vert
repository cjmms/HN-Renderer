#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


layout(location = 0) out vec3 worldNormal;

layout(push_constant) uniform Push 
{
	mat4 modelMat;
} push;

layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projViewMat;
	vec4 ambientLightColor;
	vec3 lightPos;
	vec4 lightColor;
} ubo;



void main()
{
	gl_Position = push.modelMat * vec4(position, 1.0f);
	worldNormal = normalize(vec3(push.modelMat * vec4(normal, 0)));
}