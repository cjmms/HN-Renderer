#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexture;

uniform mat4 mvp;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 Texture;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Texture = aTexture;
}






#shader fragment
#version 330 core

struct DirLight {
	vec3 dir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform vec3 CameraPos;
uniform DirLight dirLight;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

out vec4 FragColor;

vec3 calculateDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);



void main() 
{

	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(CameraPos - FragPos);


	vec3 result = vec3(0.7f);

	result = calculateDirLight(dirLight, normal, viewDir);

	FragColor = vec4(result, 1.0f);
}

// normal and viewDir are normalized already
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
	vec3 reflectDir = normalize(reflect(-light.dir, normal));
	vec3 lightDir = normalize(-light.dir);

	vec3 ambient = light.ambient * texture(material.diffuse, Texture).rgb;

	float diff = max(dot(lightDir, normal), 0);
	vec3 diffuse = diff * light.diffuse * texture(material.diffuse, Texture).rgb;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, Texture).rgb;

	return ambient + diffuse + specular;
	//return light.ambient;
}