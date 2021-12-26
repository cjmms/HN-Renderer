#pragma once
#include "pch.h"


namespace HN
{
	class Camera
	{
	public:
		void SetOrthProj(float left, float right, float top, float bottom, float near, float far);
		void SetPerspectiveProj(float fovy, float aspect, float near, float far);

		inline glm::mat4 GetProjMat() const { return projMat; };

	private:
		glm::mat4 projMat{1.0f};
	};


}