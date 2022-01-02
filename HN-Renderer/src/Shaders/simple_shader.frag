#version 450


layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec3 fragColor;

layout(push_constant) uniform Push 
{
	mat4 modelMat;
} push;

void main()
{
	FragColor = vec4(fragColor, 1);
}
  