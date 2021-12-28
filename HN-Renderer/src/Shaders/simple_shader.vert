#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push 
{
	mat4 transform;
	mat4 modelMat;
} push;

const vec3 lightDir = normalize(vec3(1.0f, -3.0f, -1.0f));
const float AMBIENT = 0.02;

void main()
{
	
	gl_Position = push.transform * vec4(position, 1.0f);

	vec3 normalWorldSpace = normalize(mat3(push.modelMat) * normal);
	

	float lightIntensity = AMBIENT + max(0, dot(normalWorldSpace, lightDir));

	fragColor = lightIntensity * vec3(1.0, 1.0, 1.0);
}