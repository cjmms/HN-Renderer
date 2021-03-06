#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;

void main() 
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
}






#shader fragment
#version 330 core

struct Material {
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 LightPos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

uniform vec3 CubeColor;
uniform vec3 CameraPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() 
{
	vec3 normal = normalize(Normal);
	// ambient
	vec3 ambient = material.ambient * light.ambient; 

	// diffuse
	vec3 ray = normalize(light.LightPos - FragPos);
	float diff = max(dot(ray, Normal), 0.0f);
	vec3 diffuse = diff * material.diffuse * light.diffuse;

	// specular
	vec3 reflectRay = reflect(-ray, Normal);
	vec3 eye = normalize(CameraPos - FragPos);
	float spec = pow(max(dot(reflectRay, eye), 0.0f), material.shininess);
	vec3 specular = spec * material.specular * light.specular;

	FragColor = vec4(CubeColor * (ambient + diffuse + specular), 1.0f);
}






