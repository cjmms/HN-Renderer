#include "Camera.h"


Camera::Camera()
	: sensitivity(0.1f), deltaTime(0.0f), lastFrame(0.0f), firstMouse(true)
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	lastX = 400;
	lastY = 300;

	yaw = -90.0f;
	pitch = 0.0f;
}



float Camera::getSensitivity() 
{
	return sensitivity;
}



void Camera::setSensitivity(float sensitivity) 
{
	this->sensitivity = sensitivity;
}



void Camera::setCameraSpeed(float speedModifier) 
{
	this->cameraSpeed = speedModifier * deltaTime;
}



float Camera::getCameraSpeed() 
{
	return cameraSpeed;	
}



void Camera::updateCameraDirection(float currentX, float currentY) 
{
	// make sure camera doesn't suddenly move at the beginning
	if (firstMouse)
	{
		lastX = currentX;
		lastY = currentY;
		firstMouse = false;
	}

	calculateCameraAngle(currentX, currentY);

	// update last x and y to current x and y
	lastX = currentX;
	lastY = currentY;

	calculateCameraFront();
}



void Camera::setCameraKey(GLFWwindow* window)
{
	setCameraSpeed(2.5f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}



void Camera::cameraUpdateFrameTime() 
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}



glm::mat4 Camera::getViewMatrix() 
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}



void Camera::calculateCameraFront() 
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}



void Camera::calculateCameraAngle(float currentX, float currentY)
{
	float offset_x = currentX - lastX;
	float offset_y = lastY - currentY;

	offset_x *= sensitivity;
	offset_y *= sensitivity;

	yaw += offset_x;
	pitch += offset_y;

	// avoid 90 degree
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}