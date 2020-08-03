#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TextureCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);

	// change z value to w, so that perspective division z will be z/w which is 1.0f
	// depth value equals perspective division z, in this case, 1.0f
	// 1.0f is the max depth value
	gl_Position = gl_Position.xyww;
	TextureCoord = aPos;
}


#shader fragment
#version 330 core

in vec3 TextureCoord;
out vec4 FragColor;

uniform samplerCube skybox;


void main()
{
	FragColor = texture(skybox, TextureCoord);
}