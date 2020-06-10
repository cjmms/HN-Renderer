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

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 direction;

	float cutoff;			// inner 
	float outterCutoff;		// outter
	float epsilon;			// cosine difference between outter cone and inner cone

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 4

uniform vec3 CameraPos;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

out vec4 FragColor;

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir);


void main() 
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(CameraPos - FragPos);

	vec3 result = vec3(0.0f);

	// directional light
	result = calculateDirLight(dirLight, normal, viewDir);

	// point light
	for (int i = 0; i < NR_POINT_LIGHTS; i++) 
		result += calculatePointLight(pointLight[i], normal, viewDir);

	result += calculateSpotLight(spotLight, normal, viewDir);

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
}



// normal and viewDir are normalized
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, normal));


	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, Texture).rgb;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, Texture).rgb;

	// specualr
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, Texture).rgb;

	return (ambient + diffuse + specular) * attenuation;
}




vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = viewDir;
	vec3 reflectDir = normalize(reflect(-lightDir, normal));

	// attenuation
	float distance = length(viewDir);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	// spotlight intensity
	float theta = dot(viewDir, normalize(-light.direction));	// cos of viewDir and light
	float intensity = clamp(((theta - light.outterCutoff) / light.epsilon), 0.0f, 1.0f);

	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, Texture).rgb;

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, Texture).rgb * intensity;

	// specualr
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, Texture).rgb * intensity;


	return (ambient + diffuse + specular) * attenuation;
}