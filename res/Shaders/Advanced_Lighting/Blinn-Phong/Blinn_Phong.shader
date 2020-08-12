#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT{
	vec3 normal;
	vec2 textureCoord;
	vec3 FragPos;
} vs_out;

uniform mat4 mvp;
uniform mat4 view;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);

	vs_out.normal =  aNormal;
	vs_out.textureCoord = aTextureCoord;
	vs_out.FragPos = aPos;
}





#shader fragment
#version 330 core

in VS_OUT{
	vec3 normal;
	vec2 textureCoord;
	vec3 FragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture_wood;
uniform vec3 viewPos;
uniform vec3 lightPos;

uniform bool Blinn;


void main()
{
	vec3 color = texture(texture_wood, fs_in.textureCoord).rgb;

	// ambient
	vec3 ambient = 0.02f * color;

	// diffuse
	vec3 normal = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * color;

	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	float spec = 0.0f;
	if (Blinn) 
		spec = pow(max(dot(halfwayDir, normal), 0.0f), 16.0f);
	else 
		spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
	
	vec3 specular = spec * vec3(0.2f);
	FragColor = vec4(specular + ambient + diffuse, 1.0f);
}