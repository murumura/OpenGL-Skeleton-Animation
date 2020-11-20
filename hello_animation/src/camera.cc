#include "camera.h"

#include <iostream>
Camera::Camera() noexcept
{
	updateVectors();
}
void Camera::set_near(const float Near)
{
	near = Near;
}

void Camera::set_far(const float Far)
{
	far = Far;
}
void Camera::set_speed(const float Speed)
{
	speed = Speed;
}
void Camera::update(const double deltaTime)
{
	if (Input::GetInstance().IsKeyPressed(GLFW_KEY_TAB)) {
		dirty = !dirty;
	}

	if (dirty) {
		// Update view from mouse movement
		updateView();

		// Update Keyboard
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_W)) {
			processKeyboard(Direction::FORWARD, deltaTime);
		}
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_S)) {
			processKeyboard(Direction::BACKWARD, deltaTime);
		}
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_A)) {
			processKeyboard(Direction::LEFT, deltaTime);
		}
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_D)) {
			processKeyboard(Direction::RIGHT, deltaTime);
		}
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_SPACE)) {
			processKeyboard(Direction::UP, deltaTime);
		}
		if (Input::GetInstance().IsKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
			processKeyboard(Direction::DOWN, deltaTime);
		}
	}
}
void Camera::updateView(const bool constrainPitch /*= true*/)
{
	// If the mouse position has changed, recalculate vectors
	if (Input::GetInstance().MouseMoved()) {
		const auto xPos = Input::GetInstance().GetMouseX();
		const auto yPos = Input::GetInstance().GetMouseY();

		if (firstMouse) {
			prevX = xPos;
			prevY = yPos;
			firstMouse = false;
		}

		const auto xOffset = (xPos - prevX) * sensitivity;
		const auto yOffset = (prevY - yPos) * sensitivity;  // Reversed since y-coordinates go from bottom to top

		prevX = xPos;
		prevY = yPos;

		yaw += xOffset;
		pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (pitch > 89.0) {
				pitch = 89.0;
			}
			if (pitch < -89.0) {
				pitch = -89.0;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateVectors();
	}
}
void Camera::updateVectors()
{
	// Calculate the new Front vector
	glm::vec3 Front{
	    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
	    front.y = glm::sin(glm::radians(pitch)),
	    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))};

	front = glm::normalize(Front);
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}
void Camera::processKeyboard(const Direction direction, const double deltaTime) noexcept
{
	const float velocity = speed * static_cast<float>(deltaTime);

	switch (direction) {
	case Direction::FORWARD:
		position += front * velocity;
		break;
	case Direction::BACKWARD:
		position -= front * velocity;
		break;
	case Direction::LEFT:
		position -= right * velocity;
		break;
	case Direction::RIGHT:
		position += right * velocity;
		break;
	case Direction::UP:
		position += worldUp * velocity;
		break;
	case Direction::DOWN:
		position -= worldUp * velocity;
		break;
	}
}