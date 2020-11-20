#ifndef TRI_H
#define TRI_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "camera.h"
#include "input.h"
#include "model.h"
#include "timer.h"
struct glfwDeleter {
	void operator()(GLFWwindow* wnd)
	{
		std::cout << "Destroying GLFW Window Context" << std::endl;
		glfwDestroyWindow(wnd);
	}
};
class Scene {
	friend class Animation;

   public:
	void init(const std::string& model_path,
	          const std::string& v_shader_path,
	          const std::string& f_shader_path, const std::string& title);
	void create_window(std::string window_title,
	                   int width = SCR_WIDTH,
	                   int height = SCR_HEIGHT,
	                   int openGLMinVersion = 3, int openGLMaxVersion = 3);
	void update();
	void process_input();
	void swap_buffers();
	void render();
	void handle_event();
	bool is_closing() const;
	void attach_camera(const glm::vec3& Position, const glm::vec3& lookAt);
	auto GetViewMatrix() const { return cam->GetViewMatrix(); }
	auto GetProjMatrix() const
	{
		return cam->GetProjMatrix(screen_width, screen_height);
	}
	~Scene()
	{
		std::cout << "Display Closing" << std::endl;
		glfwTerminate();
	}
	/*public member*/
	std::shared_ptr<Camera> cam{nullptr};
	Timer timer;

   private:
	float screen_height;
	float screen_width;
	std::unique_ptr<GLFWwindow, glfwDeleter> window;
	std::shared_ptr<Model> animation_model;
};
#endif