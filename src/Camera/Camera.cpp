#include "Camera.h"

Camera::Camera(glm::vec3 startPos, glm::vec3 startTarget, glm::vec3 worldUp, float startYaw, float startPitch, float speed, float mouseSensitivity)
{
	position = startPos;
	this->worldUp = worldUp;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::normalize(glm::cross(worldUp, front));
	up = glm::cross(front, right);

	yaw = startYaw;
	pitch = startPitch;

	this->speed = speed;
	this->mouseSensitivity = mouseSensitivity;
}

Camera::Camera() : Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f), 0, 0, 10.0f, 10.0f) {}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::HandleMovement(float deltaTime) {
	InputManager* inputMng = InputManager::GetInstance();

	float velocity = speed * deltaTime;

	if (inputMng->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		velocity *= 7;
	}
	
	if (inputMng->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		velocity /= 5;
	}

	if (inputMng->IsKeyPressed(GLFW_KEY_W))
	{
		position += velocity * front;
	}
	if (inputMng->IsKeyPressed(GLFW_KEY_S))
	{
		position -= velocity * front;
	}
	if (inputMng->IsKeyPressed(GLFW_KEY_A))
	{
		position -= glm::normalize(glm::cross(front, up)) * velocity;
	}
	if (inputMng->IsKeyPressed(GLFW_KEY_D))
	{
		position += glm::normalize(glm::cross(front, up)) * velocity;
	}
}

void Camera::HandleRotation() {
	InputManager* inputMng = InputManager::GetInstance();

	float xOffset = inputMng->GetLastMouseXMovement() * mouseSensitivity;
	float yOffset = inputMng->GetLastMouseYMovement() * mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// dont break your neck
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// i don't really get any of this math
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}