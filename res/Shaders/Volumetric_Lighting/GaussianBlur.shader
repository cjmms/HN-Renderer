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

// GLSL does not support multidimentional array
// That's why you see the devil below
// Thank you so much, GLSL!
float firstFilter[5] = float[5]( 1.0f, 4.0f, 7.0f, 4.0f, 1.0f );
float secondFilter[5] = float[5](4.0f, 16.0f, 26.0f, 16.0f, 4.0f);
float thirdFilter[5] = float[5](7.0f, 26.0f, 41.0f, 26.0f, 7.0f);
float fourthFilter[5] = secondFilter;
float fifthFilter[5] = firstFilter;


void main()
{
	vec3 color = vec3(0.0f);
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel

	for (int i = -2; i < 3; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			float GuassianFilter[5];
			if (i == -2) GuassianFilter = firstFilter;
			if (i == -1) GuassianFilter = secondFilter;
			if (i == 0) GuassianFilter = thirdFilter;
			if (i == 1) GuassianFilter = fourthFilter;
			if (i == 2) GuassianFilter = fifthFilter;


			vec2 offsets = vec2(tex_offset.x * i, tex_offset * j);
			vec3 downscaledColor = texture(image, TextureCoord + offsets).rgb;
			downscaledColor *= GuassianFilter[j + 2];
			color += downscaledColor;
		}
	}

	vec3 volumetricLight = color / 273.0f;


	FragColor = vec4(volumetricLight, 1.0f);
}