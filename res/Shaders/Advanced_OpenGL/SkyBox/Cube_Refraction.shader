#shader vertex
#version 330 core
layout(Location = 0) in vec3 aPos;
layout(Location = 2) in vec3 normal;

uniform mat4 mvp;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;

void main() {
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = mvp * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
	float ratio = 1.0 / 1.52;

	// view direction, from camera to Fragment position
	vec3 I = normalize(FragPos - cameraPos);

	// reflection vector
	vec3 R = refract(I, normalize(Normal), ratio);

	// sample skybox texture by using reflection vector
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}