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
float firstFilter[5] = float[5]( 1, 4, 7, 4, 1 );
float secondFilter[5] = float[5](4, 16, 26, 16, 4);
float thirdFilter[5] = float[5](7, 26, 41, 26, 7);
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
			float filter[5];
			if (i == -2) filter = firstFilter;
			if (i == -1) filter = secondFilter;
			if (i == 0) filter = thirdFilter;
			if (i == 1) filter = fourthFilter;
			if (i == 2) filter = fifthFilter;


			vec2 offsets = vec2(tex_offset.x * i, tex_offset * j);
			vec3 downscaledColor = texture(image, TextureCoord + offsets).rgb;
			downscaledColor *= filter[i + 2];
			color += downscaledColor;
		}
	}

	vec3 volumetricLight = color / 273;


	FragColor = vec4(volumetricLight, 1.0f);
}