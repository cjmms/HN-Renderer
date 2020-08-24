#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0f);
	texCoord = aTex;
}




#shader fragment
#version 330 core

in vec2 texCoord;


uniform sampler2D diffuseMap;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(0.2, 0.6, 0.9, 1.0);
	FragColor = vec4(texture(diffuseMap, texCoord).rgb, 1.0f);
}