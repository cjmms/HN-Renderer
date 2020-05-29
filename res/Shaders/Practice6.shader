#shader vertex
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec2 aTexture;
out vec2 textureCoord;

void main() 
{
	gl_Position = vec4(aPos, 1.0f);
	textureCoord = aTexture;
}


#shader fragment
in vec2 textureCoord;
out vec4 FragColor;

uniform sampler2D hanon;
uniform sampler2D container;

void main() 
{
	//FragColor = vec4(0.8f, 0.5f, 0.7f, 1.0f);
	FragColor = mix(texture(hanon, textureCoord), texture(container, textureCoord), 0.2f);
}