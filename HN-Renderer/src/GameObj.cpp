#include "pch.h"

#include "GameObj.hpp"




namespace HN
{
	glm::mat2 Transform2dComponent::mat2() const
	{
		glm::mat2 scaleMat = { {scale.x, 0}, {0, scale.y} };
		return scaleMat;
	}




	GameObj GameObj::CreateGameObject()
	{
		static id_t currentId = 0;
		return GameObj{ currentId++ };
	}



}