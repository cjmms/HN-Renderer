#include "ParticleSystem.h"
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>


static time_point<steady_clock> start = steady_clock::now();

ParticleSystem::ParticleSystem(int num_particles)
	:Particles(), ComputeShader("res/Shaders/ParticleSystem/ParticleSystem.cs.shader"),
	RenderShader("res/Shaders/ParticleSystem/Render.shader")
{
	for (int i = 0; i < num_particles; ++i) 
		Particles.push_back(Particle());
	
}



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
	ComputeShader.setVec2("attractor_radii", glm::vec2(400, 200));

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
