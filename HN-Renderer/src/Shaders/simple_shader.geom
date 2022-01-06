#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


layout(location = 0) in vec3 outNormal[];
layout(location = 0) out vec3 outColor[3];


layout(set = 0, binding = 0) uniform GlobalUbo
{
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPos;
	int tessellationLevel;
	vec4 lightColor;
	float height;
    float width;
	float orienFactor;
} ubo;


layout(push_constant) uniform Push 
{
	mat4 modelMat;
} push;


const vec3 darkGreen = vec3(0.0f, 0.4f, 0.0f);
const vec3 brightGreen = vec3(0.5f, 1.0f, 0.0f); 


// random number generater
// https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
float Rand(vec2 co)
{
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


float randomNumber = Rand(gl_in[0].gl_Position.xy);

void main(void)
{
	float grassHeight = 0.5 * ubo.height + ubo.height * randomNumber;


	for(int i = 0; i < gl_in.length(); i++)
	{
		vec3 worldPos = vec3(push.modelMat * gl_in[i].gl_Position);
		vec3 normal = normalize(mat3(push.modelMat) * outNormal[i]);


		vec4 viewPos = ubo.view * vec4(worldPos, 1.0);

		gl_Position = ubo.projection * (viewPos - vec4(ubo.width / 2, 0, 0, 0));	// happens inside view space
		outColor[i] = darkGreen;
		EmitVertex();

		gl_Position = ubo.projection * (viewPos + vec4(ubo.width / 2, 0, 0, 0)); 
		outColor[i] = darkGreen;
		EmitVertex();


		// generate random number from -1.0 to 1.0
		float randOrienX = (Rand(gl_in[0].gl_Position.xz) - 0.5f) * 2.0f;
		float randOrienY = (Rand(gl_in[0].gl_Position.yz) - 0.5f) * 2.0f;

		vec3 orienOffset = vec3(randOrienX * ubo.orienFactor, randOrienY * ubo.orienFactor, 0);


		gl_Position = ubo.projection * ubo.view *  vec4(worldPos + normal * grassHeight + orienOffset, 1.0);	
			
		outColor[i] = brightGreen;
		EmitVertex();

		EndPrimitive();
	}
}