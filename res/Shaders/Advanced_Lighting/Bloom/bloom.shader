#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}



#shader fragment
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D lightingScene;
uniform sampler2D brightScene;

void main()
{
	const float gamma = 2.2f;

	vec3 hdrColor = texture(lightingScene, TexCoord).rgb;
	vec3 brightColor = texture(brightScene, TexCoord).rgb;

	vec3 mappedColor = hdrColor + brightColor;

	// tone mapping
	//mappedColor = mappedColor / (mappedColor + vec3(1.0f));

	// tone mapping with exposure
	float exposure = 5.0f;
	mappedColor = vec3(1.0) - exp(-mappedColor * exposure);

	mappedColor = pow(mappedColor, vec3(1.0f / gamma));

	FragColor = vec4(mappedColor, 1.0f);
}