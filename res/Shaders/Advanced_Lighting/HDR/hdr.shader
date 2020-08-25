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


uniform sampler2D Texture;
uniform bool hdr;

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture(Texture, TexCoord).rgb;

	// reinhard tone mapping
	vec3 mappedColor = hdrColor / (hdrColor + vec3(1.0f));

	mappedColor = pow(mappedColor, vec3(1.0f / gamma));

	if (hdr) 
	{
		FragColor = vec4(mappedColor, 1.0f);
	}
	else
	{
		FragColor = vec4(hdrColor, 1.0f);
	}
}