#pragma once 

#include "pch.h"
#include "Model.hpp"





namespace HN 
{
	struct Transform2dComponent
	{
		glm::vec2 translation;
		glm::vec2 scale{1.0f, 1.0f};

		glm::mat2 mat2() const;
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
		Transform2dComponent transform2d{};

	private:
		GameObj(id_t objId) : id{objId} {}
		id_t id;

	};
}