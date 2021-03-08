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
	Particle(glm::vec2 position, glm::vec2 velocity);

	glm::vec2 position;
	glm::vec2 velocity;
	float scale;
	float mass;		
};




class ParticleSystem
{

public:
	//ParticleSystem(int num_particles = 10);
	ParticleSystem(glm::vec2 pos, glm::vec2 velocity, int num_particles = 10);

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

