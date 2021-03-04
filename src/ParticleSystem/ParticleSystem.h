#pragma once

#include "../Shader.h"
#include <vector>
// This is a 2D Particle system


struct Particle
{
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

