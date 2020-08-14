#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTextureCoord;

out VS_OUT{
	vec2 textureCoord;
	vec3 FragPos;
	vec4 lightSpaceFragPos;
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjection;
uniform mat4 lightView;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.textureCoord = aTextureCoord;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.lightSpaceFragPos = lightProjection * lightView * model * vec4(aPos, 1.0f);
	vs_out.normal = transpose(inverse(mat3(model))) * aNormal;
}






#shader fragment
#version 330 core

in VS_OUT{
	vec2 textureCoord;
	vec3 FragPos;
	vec4 lightSpaceFragPos;
	vec3 normal;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor, vec3 color)
{
	// ambient
	vec3 ambient = 0.15 * color;

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

	
	return ambient + diffuse + specular;
}


void main()
{
	vec3 color = texture(diffuseTexture, fs_in.textureCoord).rgb;
	vec3 lightColor = vec3(1.0f);
	vec3 normal = normalize(fs_in.normal);

	vec3 lighting = BlinnPhong(normal, fs_in.FragPos, lightPos, lightColor, color);

	color *= lighting;

	FragColor = vec4(color, 1.0f);
}