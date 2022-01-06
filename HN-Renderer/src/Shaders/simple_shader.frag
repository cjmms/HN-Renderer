#version 450


layout(location = 0) out vec4 FragColor;


//layout(location = 0) in vec3 fragPosWorld;
//layout(location = 1) in vec3 fragNormalWorld;



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


void main()
{

	//vec3 directionToLight = normalize(ubo.lightPos - fragPosWorld);
	//float attenuation = 1.0f / dot(ubo.lightPos - fragPosWorld, ubo.lightPos - fragPosWorld);	// atenuation

	//vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;	// point light color
	//vec3 ambientColor = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;	// ambient light color
	//vec3 diffuseLight = lightColor * max(0, dot(fragNormalWorld, directionToLight));

	//vec3 result = diffuseLight + ambientColor;

	//FragColor = vec4(result, 1);
	FragColor = vec4(0, 1, 0, 1);

}
  