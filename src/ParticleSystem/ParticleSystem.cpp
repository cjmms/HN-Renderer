#include "ParticleSystem.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>



static time_point<steady_clock> start = steady_clock::now();





void ParticleSystem::Init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	{ // Particle SSBO
		glGenBuffers(1, &SSBO);
		glBindBuffer(GL_ARRAY_BUFFER, SSBO);
		glBufferData(GL_ARRAY_BUFFER, Particles.size() * sizeof(Particle), &Particles[0], GL_STREAM_DRAW);
	}

	{ // Setup Vertex Attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);	// position
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 2)); // velocity
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 4)); // mass
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)(sizeof(float) * 5)); // scale

		glVertexAttribDivisor(0, 1);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
	}

	glBindVertexArray(0);


	// compute shader unifroms
	ComputeShader.Bind();
	float time = duration_cast<duration<float>>(steady_clock::now() - start).count();
	ComputeShader.setFloat("time", time);
	ComputeShader.setVec2("resolution", glm::vec2(1200, 1000));	// pass screen resolution
	ComputeShader.setVec2("attractor_radii", glm::vec2(300, 600));

	// render shader uniforms
	RenderShader.Bind();
	RenderShader.setInt("vertex_count", Particles.size());
	//RenderShader.setMat4("projection", glm::ortho(0.0f, (float)1200, (float)1000, 0.0f, -1.0f, 1.0f));
	RenderShader.setMat4("projection", glm::ortho(0.0f, 1200.0f, 0.0f, 1000.0f, 0.1f, 10.0f));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::ortho(0.0f, 1200.0f, 0.0f, 1000.0f, 0.0f, 10.0f);
	RenderShader.setMat4("projection", projection * view);
}



void ParticleSystem::Draw()
{
	
	// Invoke Compute Shader and wait for all memory access to SSBO to safely finish
	ComputeShader.Bind();	

	float time = duration_cast<duration<float>>(steady_clock::now() - start).count();
	ComputeShader.setFloat("time", time);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glDispatchCompute((Particles.size() / 128) + 1, 1, 1);	// group size: 128
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	

	// Render the results
	RenderShader.Bind();
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_POINTS, 0, 1, Particles.size());
	glBindVertexArray(0);
	
}



void ParticleSystem::Destory()
{
	glDeleteBuffers(1, &SSBO);
	glDeleteVertexArrays(1, &VAO);
}



void ParticleSystem::Print()
{
	std::stringstream ss;
	ss << "Compute Shader Capabilities:" << std::endl;
	ss << "GL_MAX_COMPUTE_WORK_GROUP_COUNT:" << std::endl;
	for (int i : {0, 1, 2}) {
		int tmp = 0;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &tmp);
		ss << tmp << std::endl;
	}

	ss << "GL_MAX_COMPUTE_WORK_GROUP_SIZE:" << std::endl;
	for (int i : {0, 1, 2}) {
		int tmp = 0;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &tmp);
		ss << tmp << std::endl;
	}

	ss << "Number of Particles: " << Particles.size() << std::endl;

	std::cout << ss.str() << std::endl;
}



float gen_random(float min, float max) {
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}



Particle::Particle()
	:position(gen_random(0.0, 300.0), gen_random(0.0, 300.0)),
	velocity(0.0, 0.7),
	scale(gen_random(1.0f, 16.0f)),
	mass(scale)
{}


Particle::Particle(glm::vec2 position, glm::vec2 velocity)
	:position(position),
	velocity(velocity),
	scale(gen_random(1.0f, 16.0f)),
	mass(scale)
{}


glm::vec2 randomPointInCircle(glm::vec2 center, float radius)
{
	float r = radius * sqrt(gen_random(0, 1));
	float theta = gen_random(0, 1) * 2 * 3.1415f;

	float x = center[0] + r * cosf(theta);
	float y = center[1] + r * sinf(theta);

	return glm::vec2(x, y);
}

glm::vec2 randomPointInSquare(glm::vec2 center, float length)
{
	float posX = gen_random(center[0] - length, center[0] + length);
	float posY = gen_random(center[1] - length, center[1] + length);
	return glm::vec2(posX, posY);
}



glm::vec2 GenRandomCircSectorDir(float angle1, float angle2)
{
	float angle = gen_random(angle1, angle2);	// random angle from 0 to 2 PI
	return glm::vec2(cosf(angle), sinf(angle));
}


glm::vec2 GenRandomCircDir()
{
	return GenRandomCircSectorDir(0.0f, 2.0f * 3.1415f);
}

float CalVecAngle(glm::vec2 vec)
{
	float angle = atan2f(vec[1], vec[0]);;
	//if (angle < 0) { angle += 2 * 3.1415f; }
	return angle;
}



ParticleSystem::ParticleSystem(SpawnConfig spawnConfig, MoveConfig moveConfig, int num_particles)
	:Particles(), ComputeShader("res/Shaders/ParticleSystem/ParticleSystem.cs.shader"),
	RenderShader("res/Shaders/ParticleSystem/Render.shader")

{
	for (int i = 0; i < num_particles; ++i) {
		glm::vec2 pos(0.0f);
		glm::vec2 velocity(0.0f);

		if (spawnConfig.mode == SQUARE)
			pos = randomPointInSquare(spawnConfig.areaCenter, spawnConfig.length);
		else if (spawnConfig.mode == CIRCLE)
			pos = randomPointInCircle(spawnConfig.areaCenter, spawnConfig.length);

		if (moveConfig.mode == LINEAR)
			velocity = moveConfig.direction * moveConfig.magnitude;
		else if (moveConfig.mode == CIRCULAR)
			velocity = GenRandomCircDir() * moveConfig.magnitude;
		else if (moveConfig.mode == CIRCULAR_SECTOR)
		{
			float angle = CalVecAngle(moveConfig.direction);

			float minAngle = angle - moveConfig.angle / 2.0f;
			float maxAngle = angle + moveConfig.angle / 2.0f;
			velocity = GenRandomCircSectorDir(minAngle, maxAngle) * moveConfig.magnitude;
		}

		Particles.push_back(Particle(pos, velocity));
	}
}


ParticleSystem::ParticleSystem(int num_particles)
	:Particles(), ComputeShader("res/Shaders/ParticleSystem/ParticleSystem.cs.shader"),
	RenderShader("res/Shaders/ParticleSystem/Render.shader")

{
	for (int i = 0; i < num_particles; ++i)
		Particles.push_back(Particle());
}