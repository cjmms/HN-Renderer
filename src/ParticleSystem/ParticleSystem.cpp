#include "ParticleSystem.h"
#include <sstream>
#include <string>
#include <iostream>


void ParticleSystem::Init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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

	{ // Particle SSBO
		glGenBuffers(1, &SSBO);
		glBindBuffer(GL_ARRAY_BUFFER, SSBO);
		glBufferData(GL_ARRAY_BUFFER, Particles.size() * sizeof(Particle), &Particles[0], GL_STREAM_DRAW);
	}

	glBindVertexArray(0);


	// TODO
	// shader uniforms setup

	// compute shader unifroms


	// render shader uniforms

}


void ParticleSystem::Draw()
{
	// Invoke Compute Shader and wait for all memory access to SSBO to safely finish
	ComputeShader.Bind();

	// pass elapsed time as uniform
	// TODO
	ComputeShader.setFloat("time", );
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