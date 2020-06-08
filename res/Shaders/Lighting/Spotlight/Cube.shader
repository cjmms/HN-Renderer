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

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 LightPos;
	vec3 direction;
	float cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 CameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 eye = normalize(CameraPos - FragPos);
	vec3 ray = eye;
	vec3 reflectRay = reflect(-ray, normal);

	// attenuation
	float distance = length(eye);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, Texture).rgb;

	float theta = dot(eye, normalize(-light.direction));

	if (light.cutoff < theta)
	{
		// diffuse
		float diff = max(dot(ray, normal), 0.0f);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, Texture).rgb;

		// specualr
		float spec = pow(max(dot(reflectRay, eye), 0.0f), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, Texture).rgb;

		FragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0f);
	}
	else 
	{
		FragColor = vec4(ambient * attenuation, 1.0f);
	}
}


