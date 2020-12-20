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

uniform sampler2D depthMap;
uniform sampler2D image;

const int BlurSamples = 5;





void main()
{
	vec3 color = vec3(0.0f);
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel

	for (int i = 0; i < BlurSamples; i++)
	{
		for (int j = 0; j < BlurSamples; j++)
		{
			vec2 offsets = vec2(tex_offset.x * i, tex_offset * j);
			vec3 downscaledColor = texture(image, TextureCoord + offsets).rgb;
			color += downscaledColor;
		}
	}

	vec3 volumetricLight = color / float(BlurSamples * BlurSamples);


	FragColor = vec4(volumetricLight, 1.0f);
}