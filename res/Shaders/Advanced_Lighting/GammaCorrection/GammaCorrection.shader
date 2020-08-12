#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT {
	vec3 FragPos;
	vec3 normal;
	vec2 textureCoord;
} vs_out;

uniform mat4 view;
uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);

	vs_out.FragPos = aPos;
	vs_out.normal = aNormal;
	vs_out.textureCoord = aTextureCoord;
}





#shader fragment
#version 330 core


in VS_OUT {
	vec3 FragPos;
	vec3 normal;
	vec2 textureCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture_wood;
uniform vec3 viewPos;

uniform bool GammaEnabled;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 64.0f);

	vec3 specular = spec * lightColor;

	// attenuation
	float max_distance = 1.5;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (GammaEnabled ? distance * distance : distance);

	diffuse *= attenuation;
	specular *= attenuation;
	return diffuse + specular;
}



void main()
{
	vec3 color = texture(texture_wood, fs_in.textureCoord).rgb;

	vec3 lighting = vec3(0.0);

	for (int i = 0; i < 4; ++i)
		lighting += BlinnPhong(normalize(fs_in.normal), fs_in.FragPos, lightPositions[i], lightColors[i]);

	color *= lighting;

	if (GammaEnabled) color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color, 1.0f);
}