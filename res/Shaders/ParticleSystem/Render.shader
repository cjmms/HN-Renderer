#shader vertex
#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 velocity;
layout(location = 2) in float scale;
layout(location = 3) in float duration;


out float vertex_scale;
out float vertex_speed;
out float vertex_duration;

uniform float vertex_count;

void main() {	
	gl_Position = vec4(position, 0.2f, 1.0);	// 0.2 is a bias

	vertex_scale = scale;
	vertex_speed = length(velocity);
	vertex_duration = duration;
};




#shader geometry
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in float vertex_scale[];
in float vertex_speed[];
in float vertex_duration[];

out vec2 geom_uv;
out float geom_speed;
out float geom_duration;

void main() {
	const vec4 position = gl_in[0].gl_Position;
	const float speed = vertex_speed[0];
	const float size = vertex_scale[0] * 0.02;

	geom_speed = speed;
	geom_duration = vertex_duration[0];

	geom_uv = vec2(0, 0);
	gl_Position = projection * (position + vec4(-size, -size, 0.0, 0.0));
	EmitVertex();

	geom_uv = vec2(0, 1);
	gl_Position = projection * (position + vec4(-size, size, 0.0, 0.0));
	EmitVertex();

	geom_uv = vec2(1, 0);
	gl_Position = projection * (position + vec4(size, -size, 0.0, 0.0));
	EmitVertex();

	geom_uv = vec2(1, 1);
	gl_Position = projection * (position + vec4(size, size, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();
}



#shader fragment
#version 450

in vec2 geom_uv;
in float geom_speed;
in float geom_duration;

out vec4 FragColor;

void main() {
	float c = geom_speed * 0.3;

	vec4 color = vec4(-0.1 + c, 0.3, 1.4 - c, 0.1);	// color base on speed

	FragColor = color;

	if (geom_duration < 0.0) { discard; }
};