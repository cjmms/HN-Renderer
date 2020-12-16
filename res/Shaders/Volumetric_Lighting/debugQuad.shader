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


void main()
{
	FragColor = texture(depthMap, TextureCoord);
}