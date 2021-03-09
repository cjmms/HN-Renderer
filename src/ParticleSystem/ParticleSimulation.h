#pragma once

#include "../Shader.h"
#include "../Camera.h"
#include "../Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParticleSystem.h"




class ParticleSimulator
{
    


public:
    ParticleSystem* CreateParticleEffect(glm::vec2 dir, float mag, int num);


   // glm::vec2 GenRandomCircSectorDir(float angle1, float angle2);
   // glm::vec2 GenRandomCircDir();
};

 

int runPariticleSimulation();