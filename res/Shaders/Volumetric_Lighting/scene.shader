#shader vertex
#version 330 core


layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT{
	vec2 textureCoord;
	vec3 FragPos;
	vec3 normal;
	vec4 lightSpaceFragPos;
	vec4 worldPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightView;
uniform mat4 lightProjection;



void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	//gl_position = lightProjection * lightView * model * vec4(aPos, 1.0f);

	vs_out.textureCoord = aTextureCoord;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.normal = transpose(inverse(mat3(model))) * aNormal;


	vs_out.lightSpaceFragPos = lightProjection * lightView * model * vec4(aPos, 1.0f);

	vs_out.worldPos = model * vec4(aPos, 1.0f);
}












#shader fragment
#version 330 core

in VS_OUT{
	vec2 textureCoord;
	vec3 FragPos;
	vec3 normal;
	vec4 lightSpaceFragPos;
	vec4 worldPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform sampler2D depthMap;

uniform mat4 lightView;
uniform mat4 lightProjection;


uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float farPlane;

const float G_SCATTERING = 0.45f;
const float PI = 3.141f;
const int NB_STEPS = 100;

const float light_constant = 1.0f;
const float light_linear = 0.09f;
const float light_quadratic = 0.032f;

// bayer matrix
const float ditherPattern[16] = float[](	0.0f, 0.5f, 0.125f, 0.625f,
										0.75f, 0.22f, 0.875f, 0.375f,
										0.1875f, 0.6875f, 0.0625f, 0.5625,
										0.9375f, 0.4375f, 0.8125f, 0.3125	);

float calculateDitherValue(vec2 pixel)
{
	int index_a = int(pixel.x) % 4;
	int index_b = int(pixel.y) % 4;
	return ditherPattern[index_a + index_b * 4];
}



// Mie scaterring approximated with Henyey-Greenstein phase function.
float ComputeScattering(float lightDotView)
{
	float result = 1.0f - G_SCATTERING * G_SCATTERING;
	result /= (4.0f * PI * pow(1.0f + G_SCATTERING * G_SCATTERING - (2.0f * G_SCATTERING) * lightDotView, 1.5f));
	return result;
}


vec3 calculateVolumetricLighting()
{
	// view Ray
	vec3 viewRay = fs_in.FragPos - viewPos;
	float rayLength = length(viewRay);
	vec3 viewRayDir = viewRay / rayLength;

	// Step length
	float stepLength = rayLength / NB_STEPS;
	//float stepLength = 0.01f;
	vec3 step = viewRayDir * stepLength;	// point from camera to pixel

	// init sampleing position and accumlated fog value
	vec3 currentPos = viewPos;
	vec3 accumFog = vec3(0.0);

	
	currentPos += step * calculateDitherValue(gl_FragCoord.xy);
	
	// sampling along with viewRay
	for (int i = 0; i < NB_STEPS; ++i)
		//for (int i = 0; i < rayLength / stepLength; ++i)
	{
		vec4 InLightWorldSpace = lightProjection * lightView * vec4(currentPos, 1.0f);

		vec4 sampleInLightWorldSpace = InLightWorldSpace / InLightWorldSpace.w;
		
		// transform to [0,1] range
		sampleInLightWorldSpace = sampleInLightWorldSpace * 0.5 + 0.5;


		if (InLightWorldSpace.w <= 0) continue;
		//if (sampleInLightWorldSpace.x > 1) continue;
		//if (sampleInLightWorldSpace.y > 1) continue;
		//if (sampleInLightWorldSpace.x < 0) continue;
		//if (sampleInLightWorldSpace.y < 0) continue;
		
		float depthMapDepth = texture(depthMap, sampleInLightWorldSpace.xy).r;

		if ((depthMapDepth) > sampleInLightWorldSpace.z)
		{
			vec3 sunDir = normalize(lightPos - currentPos);
			accumFog += ComputeScattering(dot(viewRayDir, sunDir)) * vec3(8.0f) / NB_STEPS;
		}
		
		currentPos += step;	// move to next sample
		//currentPos += step * calculateDitherValue(currentPos);	// move to next sample
	}

	return accumFog ;
}






float calculateShadow(vec3 normal, vec3 lightDir)
{
	vec3 projCoord = fs_in.lightSpaceFragPos.xyz / fs_in.lightSpaceFragPos.w;

	// transform to [0,1] range
	projCoord = projCoord * 0.5 + 0.5;

	if (fs_in.lightSpaceFragPos.w <= 0) return 0;
	if (projCoord.x > 1) return 0;
	if (projCoord.y > 1) return 0;
	if (projCoord.x < 0) return 0;
	if (projCoord.y < 0) return 0;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(depthMap, projCoord.xy).r;

	// get depth of current fragment from light's perspective
	float currentDepth = projCoord.z;

	float bias = 0.01f;

	// check whether current frag pos is in shadow
	float shadow = currentDepth > (closestDepth + bias) ? 1.0 : 0.0;

	return shadow;
}



vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor, vec3 color)
{
	// ambient
	vec3 ambient = 0.1 * color;

	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);

	vec3 specular = spec * lightColor;

	// shadow
	float shadow = 1 - calculateShadow(normal, lightDir);

	vec3 volumetric_lighting = calculateVolumetricLighting();

	// shadow is not completely dark, ambient should lit shadow area
	return ambient + shadow * (diffuse + specular + volumetric_lighting);
	//return ambient + shadow * (diffuse + specular );
}



void main()
{
	
	vec3 color = texture(diffuseMap, fs_in.textureCoord).rgb;
	vec3 lightColor = vec3(0.9f);
	vec3 normal = normalize(fs_in.normal);

	vec3 lighting = BlinnPhong(normal, fs_in.FragPos, lightPos, lightColor, color);

	// attenuation
	float distance = length(fs_in.FragPos - viewPos);
	float attenuation = 1.0 / (light_constant + light_linear * distance +
		light_quadratic * (distance * distance));

	//lighting *= attenuation;

	//color *= lighting;
	color *= calculateVolumetricLighting();

	// no Gamma Correction
	FragColor = vec4(color, 1.0f);
}