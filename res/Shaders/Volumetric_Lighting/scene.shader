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
uniform int enableDithering;


uniform vec3 lightPos;
uniform vec3 viewPos;


const float G_SCATTERING = 0.45f;
const float PI = 3.141f;
const int NB_STEPS = 15;



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

	if (enableDithering == 1)
		currentPos += step * calculateDitherValue(gl_FragCoord.xy);

	// sampling along with viewRay
	for (int i = 0; i < NB_STEPS; ++i)
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
			accumFog += ComputeScattering(dot(viewRayDir, sunDir)) * vec3(10.0f) / NB_STEPS;
		}
		
		currentPos += step;	// move to next sample
	}

	return accumFog ;
}



void main()
{
	vec3 color = texture(diffuseMap, fs_in.textureCoord).rgb;

	color *= calculateVolumetricLighting();

	// no Gamma Correction
	FragColor = vec4(color, 1.0f);
}