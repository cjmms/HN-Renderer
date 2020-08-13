#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT{
	vec2 textureCoord;
} vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.textureCoord = aTextureCoord;
}






#shader fragment
#version 330 core

in VS_OUT{
	vec2 textureCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture_0;

void main()
{
	FragColor = texture(texture_0, fs_in.textureCoord);
}