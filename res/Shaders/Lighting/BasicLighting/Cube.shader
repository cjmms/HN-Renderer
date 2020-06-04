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
	// Normal = mat3(transpose(inverse(model))) * aNormal;
	Normal = mat3(model) * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
}


#shader fragment
#version 330 core

uniform vec3 CubeColor;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 CameraPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 ray = normalize(LightPos - FragPos);

	vec3 viewDir = normalize(CameraPos - FragPos);
	vec3 reflectDir = reflect(-ray, normal);

	float specModifier = 0.5f;

	float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = LightColor * specularStrength * specModifier;

	float diffuseStrength = max(dot(normal, ray), 0);
	vec3 diffuse = LightColor * diffuseStrength;

	float ambientStrength = 0.1f;
	vec3 ambient = LightColor * ambientStrength;

	FragColor = vec4(CubeColor * (ambient + diffuse + specular), 1.0f);
}