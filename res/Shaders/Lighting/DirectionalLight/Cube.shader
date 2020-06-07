#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexture;

uniform mat4 mvp;
uniform mat4 model;

out vec3 Normal;
out vec2 Texture;
out vec3 FragPos;

void main() 
{
	FragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = mvp * vec4(aPos, 1.0f);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Texture = aTexture;
}





#shader fragment
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 LightDirection;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 CameraPos;

in vec3 Normal;
in vec2 Texture;
in vec3 FragPos;

out vec4 FragColor;

void main() {
	vec3 ray = normalize(-light.LightDirection);
	//vec3 ray = normalize(light.LightDirection - FragPos);
	vec3 normal = normalize(Normal);
	vec3 reflectRay = reflect(-ray, normal);
	vec3 eye = normalize(CameraPos - FragPos);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, Texture));
	
	float diff = max(dot(ray, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, Texture));

	float spec = pow(max(dot(reflectRay, eye), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, Texture));

	FragColor = vec4(specular + diffuse + ambient, 1.0);
}
