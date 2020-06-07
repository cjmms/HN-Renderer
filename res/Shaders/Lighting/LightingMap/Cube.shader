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
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Texture = aTexture;
}




#shader fragment
#version 330 core

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 LightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform Material material;
uniform Light light;

uniform vec3 CameraPos;
uniform vec3 CubeColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

out vec4 FragColor;

void main() 
{
	vec3 normal = normalize(Normal);
	vec3 ray = normalize(light.LightPos - FragPos);
	vec3 reflectRay = reflect(-ray, normal);
	vec3 eye = normalize(CameraPos - FragPos);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, Texture));

	// diffuse
	float diff = max(dot(ray, normal), 0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, Texture));

	// specular
	float spec = pow(max(dot(reflectRay, eye), 0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}




