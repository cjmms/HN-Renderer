#include "pch.h"

#include "GameObj.hpp"




namespace HN
{
	GameObj GameObj::CreateGameObject()
	{
		static id_t currentId = 0;
		return GameObj{ currentId++ };
	}



}