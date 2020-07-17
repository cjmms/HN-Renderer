#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexture;

uniform mat4 mvp;

out vec2 Texture;

void main() 
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	Texture = aTexture;
}


#shader fragment
#version 330 core

in vec2 Texture;
out vec4 FragColor;

uniform sampler2D texture0;
 
float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	FragColor = vec4(vec3(depth), 1.0);
	//FragColor = texture(texture0, Texture);
}
