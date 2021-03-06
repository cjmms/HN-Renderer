#shader compute
#version 450 core

struct Particle {
	vec2 position;
	vec2 velocity;
	float scale;
	float duration;
};

layout(std430, binding = 0) buffer Particles {
	Particle particles[];
};

layout(local_size_x = 128) in;

uniform float time;
uniform vec2 resolution;
uniform vec2 attractor_radii;


// not Law of Universal Gravitation
//vec2 calculate_gravity(uint gid, float attractor_mass, vec2 attractor_location) {
//	float d = distance(attractor_location, particles[gid].position);
//	float m = (attractor_mass * particles[gid].mass) / d;
//	return (attractor_location - particles[gid].position) * m;
//}

void main() {
	uint gid = gl_GlobalInvocationID.x;
	
	// Attractor position and offset
	vec2 p = 0.5 * resolution;
	vec2 a = vec2(cos(time), sin(time)) * attractor_radii;

	// Attractor #1
	//vec2 g1 = calculate_gravity(gid, 0.0001, p + (0.6*a));
	//particles[gid].velocity += g1 * particles[gid].mass;	// use time

	// Attractor #2
	//vec2 g2 = calculate_gravity(gid, 0.0001, p + (0.1*a));
	//particles[gid].velocity += g2 * particles[gid].mass;

	// update duration 
	particles[gid].duration -= time;	// time is time passed between 2 frames

	// Update position from velocity
	particles[gid].position += particles[gid].velocity;
	
}