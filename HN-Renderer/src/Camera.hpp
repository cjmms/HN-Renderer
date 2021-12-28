#pragma once
#include "pch.h"


namespace HN
{
	class Camera
	{
	public:
		void SetOrthProj(float left, float right, float top, float bottom, float near, float far);
		void SetPerspectiveProj(float fovy, float aspect, float near, float far);

		void SetViewDir(glm::vec3 position, glm::vec3 dir, glm::vec3 up = glm::vec3{0.0f, -1.0f, 0.0f});
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		inline glm::mat4 GetProjMat() const { return projMat; };
		inline glm::mat4 GetViewMat() const { return viewMat; }
		
	private:
		glm::mat4 projMat{1.0f};
		glm::mat4 viewMat{1.0f};
	};


}