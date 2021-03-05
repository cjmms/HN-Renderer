#pragma once

#include "../Shader.h"
#include <vector>
#include <chrono>
#include <random>
// This is a 2D Particle system

using namespace std::chrono;


float gen_random(float min, float max);


struct Particle
{
	Particle()
		:position(gen_random(-1.0, 1.0), gen_random(-1.0, 1.0)),
		velocity(0.0f),
		scale(gen_random(0.1f, 1.0f)),
		mass(scale)
	{
		std::cout << "Particle position: " << position.x << ", " << position.y << std::endl;
	}

	glm::vec2 position;
	glm::vec2 velocity;
	float mass;
	float scale;		
};




class ParticleSystem
{

public:
	ParticleSystem(int num_particles = 10);

	void Init();
	void Draw();
	void Destory();
	void Print();


private:
	unsigned int VAO;
	unsigned int SSBO;

	Shader RenderShader;
	Shader ComputeShader;

	std::vector<Particle> Particles;
};

