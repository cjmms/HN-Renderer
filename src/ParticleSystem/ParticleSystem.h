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
	Particle();
	Particle(glm::vec2 position, glm::vec2 velocity);

	glm::vec2 position;
	glm::vec2 velocity;
	float scale;
	float mass;		
};

enum AREA_MODE
{
	CIRCLE,
	SQUARE
};

enum DIR_MODE
{
	CIRCULAR,
	LINEAR
};

struct SpawnConfig
{
	SpawnConfig(glm::vec2 center, float length, AREA_MODE mode)
		:areaCenter(center), length(length), mode(mode){}

	glm::vec2 areaCenter;
	float length;
	AREA_MODE mode;
};


struct MoveConfig
{
	MoveConfig(glm::vec2 dir, float magnitude, DIR_MODE mode)
		:direction(glm::normalize(dir)), magnitude(magnitude), mode(mode) {}

	glm::vec2 direction;
	float magnitude;
	DIR_MODE mode;
};




class ParticleSystem
{

public:
	ParticleSystem(int num_particles = 10);
	ParticleSystem(SpawnConfig spawnConfig, MoveConfig moveConfig, int num_particles = 10);

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

