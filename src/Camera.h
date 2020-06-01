#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	// camera direction = cameraFront - cameraPos
	glm::vec3 cameraPos, cameraFront, cameraUp;

	// delta time: time difference from last frame to current frame
	// last Frame: time of last frame
	float deltaTime, lastFrame;

	// x and y position from last frame
	float lastX, lastY;
	float yaw, pitch;

	float sensitivity;
	float cameraSpeed;

public:
	Camera();
	float getSensitivity();
	void setSensitivity(float sensitivity);
	void changeCameraDirection(float currentX, float currentY);
	void setCameraSpeed(float speed);
	float getCameraSpeed();
	void setCameraKey(GLFWwindow* window);
	void cameraUpdateFrameTime();
	glm::mat4 getViewMatrix();
};

