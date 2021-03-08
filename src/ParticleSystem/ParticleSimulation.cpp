
#include "ParticleSimulation.h"



ParticleSystem ParticleSimulator::CreateParticleEffect(glm::vec2 dir, glm::vec2 mag)
{
	glm::vec2 pos(gen_random(0.0, 300.0), gen_random(0.0, 300.0));


	return ParticleSystem(pos, dir * mag, 10000);

}
