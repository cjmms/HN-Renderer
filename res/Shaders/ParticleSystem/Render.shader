#shader vertex
#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 velocity;
layout(location = 2) in float scale;
layout(location = 3) in float mass;

out float vertex_scale;
out float vertex_speed;

uniform float vertex_count;

void main() {
	float depth_bias = ((2.0 / vertex_count) * gl_VertexID) - 1.0;
	gl_Position = vec4(position, depth_bias, 1.0);
	gl_Position = vec4(position, 0.0f, 1.0);

	vertex_scale = scale;
	vertex_speed = length(velocity);
};




#shader geometry
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in float vertex_scale[];
in float vertex_speed[];

out vec2 geom_uv;
out float geom_speed;

void main() {
	const vec4 position = gl_in[0].gl_Position;
	const float speed = vertex_speed[0];
	const float scale = vertex_scale[0] * 0.2;

	geom_speed = speed;

	float size = 0.002;

	//gl_Position = projection * (position + vec4(-scale, -scale, 0, 0));
	gl_Position = position + vec4(-size, -size, 0.0, 0.0);
	geom_uv = vec2(0, 0);
	EmitVertex();

	//gl_Position = projection * (position + vec4(-scale, scale, 0, 0));
	gl_Position = position + vec4(-size, size, 0.0, 0.0);
	geom_uv = vec2(0, 1);
	EmitVertex();

	//gl_Position = projection * (position + vec4(scale, -scale, 0, 0));
	gl_Position = position + vec4(size, -size, 0.0, 0.0);
	geom_uv = vec2(1, 0);
	EmitVertex();

	//gl_Position = projection * (position + vec4(scale, scale, 0, 0));
	gl_Position = position + vec4(size, size, 0.0, 0.0);
	geom_uv = vec2(1, 1);
	EmitVertex();

	EndPrimitive();
}



#shader fragment
#version 450

in vec2 geom_uv;
in float geom_speed;

out vec4 FragColor;

void main() {
	float d = distance(geom_uv, vec2(0.5, 0.5));
	float c = geom_speed * 0.3;

	vec4 color = vec4(-0.1 + c, 0.3, 1.4 - c, 0.1);
	color.a *= pow((1.0 - (2.0 * d)), 0.5);
	
	color = vec4(1.0, 0.0, 0.0, 1.0);

	FragColor = color;
};