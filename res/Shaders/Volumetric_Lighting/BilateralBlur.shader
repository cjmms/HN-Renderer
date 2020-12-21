#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TextureCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TextureCoord = aTextureCoord;
}





#shader fragment
#version 330 core

in vec2 TextureCoord;
out vec4 FragColor;

uniform sampler2D image;
uniform sampler2D depthMap;


void main()
{
	float upSampledDepth = texture(depthMap, TextureCoord).r;

	vec3 color = vec3(0.0f);
	float totalWeight = 0.0f;

	// Select the closest downscaled pixels.
	int xOffset = int(gl_FragCoord.x) % 2 == 0 ? -1 : 1;
	int yOffset = int(gl_FragCoord.y) % 2 == 0 ? -1 : 1;

	vec2 offsets = vec2(0.0f);

	vec2 tex_offset = 1.0 / textureSize(image, 0);

	for (int i = 0; i < 4; i++)
	{
		if (i == 1) offsets = vec2(0.0f, yOffset * tex_offset.y);
		if (i == 2) offsets = vec2(xOffset * tex_offset.x, 0.0f);
		if (i == 3) offsets = vec2(xOffset * tex_offset.x, yOffset * tex_offset.y);

		vec3 downscaledColor = texture(image, TextureCoord + offsets).rgb;
		float downscaledDepth = texture(depthMap, TextureCoord + offsets).r;

		float currentWeight = max(0.0f, 1.0f - (0.05f) * abs(downscaledDepth - upSampledDepth));

		color += downscaledColor * currentWeight;
		totalWeight += currentWeight;
	}


	const float epsilon = 0.0001f;
	vec3 volumetricLight = color / (totalWeight + epsilon);

	FragColor = vec4(volumetricLight, 1.0f);
}