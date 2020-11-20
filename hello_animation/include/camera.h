#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>

#include "input.h"
#include "typedef.h"

class Camera {
   public:
	Camera() noexcept;
	void set_near(const float Near);
	void set_far(const float Far);
	void set_speed(const float Speed);
	void update(const double deltaTime);
	auto GetViewMatrix() const { return glm::lookAt(position, position + front, up); }
	auto GetProjMatrix(const float width, const float height) const { return glm::perspective(FOV, width / height, near, far); }
	auto GetPosition() const noexcept { return position; }
	template <typename... Args>
	static std::shared_ptr<Camera>
	create(Args... args)
	{
		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
		camera->build(args...);
		return camera;
	}
	template <typename... InitPositions>
	void build(InitPositions... pos_args)
	{
		_build(pos_args...);
	}
	/*internal helper function for different type of constructor*/
	template <typename... InitPositions>
	void _build(const glm::vec3& Position, const glm::vec3& lookAt, InitPositions... args)
	{
		position = Position;
		target = lookAt;
	}
	void _build()
	{
	}
	enum class Direction {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	friend class Scene;

   protected:
	// Processes input received from mouse.
	void updateView(const bool constrainPitch = true);
	// Processes input received from keyboard.
	void processKeyboard(const Direction direction, const double deltaTime) noexcept;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateVectors();

	// Camera Attributes
	glm::vec3 front{0.0f, 0.0f, -1.0f};
	glm::vec3 position;
	glm::vec3 target;  //target position
	glm::vec3 up{0.0f, 1.0f, 0.0f};
	glm::vec3 right;

	const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

	float near = 1.0f, far = 100.0f;

	// Eular Angles
	float yaw{-90.0f};
	float pitch{0.0f};

	float speed{2.0f};
	const float sensitivity{0.1f};
	const float FOV{70.0f};
	const float aspects{SCR_WIDTH / SCR_HEIGHT};
	// Mouse positions
	bool firstMouse{true};
	double prevX{0.0}, prevY{0.0};

	// Should we update the camera attributes?
	bool dirty{true};
};
#endif