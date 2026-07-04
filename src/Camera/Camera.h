#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../InputManager/InputManager.h"
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 startPos, glm::vec3 startTarget, glm::vec3 worldUp, float startYaw, float startPitch, float speed, float mouseSensitivity);
	Camera();

	glm::mat4 CalculateViewMatrix();

	void HandleMovement(float deltaTime);
	void HandleRotation();

	glm::vec3 GetPosition() { return position; };

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float yaw;
	float pitch;

	float speed;
	float mouseSensitivity;

};
