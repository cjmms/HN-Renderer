#pragma once 

#include "pch.h"
#include "Model.hpp"





namespace HN 
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};

		glm::vec3 rotation{};
		glm::mat4 mat4() const;
	};


	class GameObj
	{
	public:
		using id_t = unsigned int;

		static GameObj CreateGameObject();

		// no copy constructor
		GameObj(const GameObj&) = delete;
		GameObj& operator=(const GameObj&) = delete;

		// move and assignment constructor as default
		GameObj(GameObj&&) = default;
		GameObj& operator=(GameObj&&) = default;

		inline id_t GetId() const { return id; }

	public:
		std::shared_ptr<Model> model{};
		glm::vec3 color;
		TransformComponent transform{};

	private:
		GameObj(id_t objId) : id{objId} {}
		id_t id;

	};
}