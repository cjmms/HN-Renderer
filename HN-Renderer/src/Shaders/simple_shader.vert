#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

layout(location = 0) out vec3 fragColor;

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
	vec4 positionWorld = push.modelMat * vec4(position, 1.0f);

	gl_Position = ubo.projViewMat * positionWorld;
	vec3 normalWorldSpace = normalize(mat3(push.modelMat) * normal);
	
	vec3 directionToLight = normalize(ubo.lightPos - positionWorld.rgb);
	float attenuation = 1.0f / dot(ubo.lightPos - positionWorld.rgb, ubo.lightPos - positionWorld.rgb);	// atenuation

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;	// point light color
	vec3 ambientColor = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;	// ambient light color
	vec3 diffuseLight = lightColor * max(0, dot(normalWorldSpace, directionToLight));

	fragColor = diffuseLight + ambientColor;
}