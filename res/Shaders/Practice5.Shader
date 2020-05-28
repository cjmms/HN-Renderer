#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aColor;
layout(Location = 2) in vec2 aTexture;
out vec4 vertColor;
out vec2 vertTexture;

uniform mat4 transform;

void main() 
{
	gl_Position = transform * vec4(aPos, 1.0f);
	vertColor = vec4(aColor, 1.0f);
	vertTexture = aTexture;
}



#shader fragment
#version 330 core
in vec4 vertColor;
in vec2 vertTexture;
out vec4 FragColor;

uniform sampler2D hanon;
uniform sampler2D container;

void main() 
{
	//FragColor = vec4(0.6f, 0.2f, 0.9f, 1.0f);
	FragColor = mix(texture(hanon, vertTexture), texture(container, vertTexture), 0.2);
}