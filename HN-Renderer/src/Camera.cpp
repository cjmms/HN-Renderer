#include "pch.h"
#include "Camera.hpp"


namespace HN
{
	void Camera::SetOrthProj(float left, float right, float top, float bottom, float near, float far)
	{
		projMat = glm::mat4{ 1.0f };
		projMat[0][0] = 2.f / (right - left);
		projMat[1][1] = 2.f / (bottom - top);
		projMat[2][2] = 1.f / (far - near);
		projMat[3][0] = -(right + left) / (right - left);
		projMat[3][1] = -(bottom + top) / (bottom - top);
		projMat[3][2] = -near / (far - near);
	}



	void Camera::SetPerspectiveProj(float fovy, float aspect, float near, float far)
	{
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projMat = glm::mat4{ 0.0f };
		projMat[0][0] = 1.f / (aspect * tanHalfFovy);
		projMat[1][1] = 1.f / (tanHalfFovy);
		projMat[2][2] = far / (far - near);
		projMat[2][3] = 1.f;
		projMat[3][2] = -(far * near) / (far - near);
	}






}