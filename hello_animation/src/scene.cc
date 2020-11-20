#include "scene.h"

#include <iostream>
#include <vector>
void Scene::init(const std::string& model_path,
                 const std::string& v_shader_path,
                 const std::string& f_shader_path, const std::string& title)
{
	create_window(title);
	animation_model = Model::create(
	    model_path,
	    v_shader_path,
	    f_shader_path);
}
void Scene::create_window(const std::string window_title, int width, int height,
                          int openGLMinVersion, int openGLMaxVersion)
{
	if (!glfwInit()) {
		std::cerr << "Failed to start GLFW.";
		std::abort();
	}
	//glfw settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLMaxVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLMinVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	/*unique_ptr :reset():replaces the managed object*/
	this->window.reset(glfwCreateWindow(screen_width = width, screen_height = height, window_title.c_str(), nullptr, nullptr));

	glfwMakeContextCurrent(this->window.get());
	glfwSetWindowUserPointer(this->window.get(), this);

	glfwSetWindowSizeCallback(this->window.get(), [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		Input::GetInstance().windowResized(width, height);
	});
	glfwSetKeyCallback(this->window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mode) {
		Input::GetInstance().keyPressed(key, scancode, action, mode);
	});
	glfwSetCursorPosCallback(this->window.get(), [](GLFWwindow* window, double xPos, double yPos) {
		Input::GetInstance().mouseMoved(xPos, yPos);
	});

	/*check if we have a pointer to the window*/
	if (this->window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	//load all openGL function pointer
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to start GLAD.";
		std::abort();
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Setup the OpenGL Viewport
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);
}
void Scene::update()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	timer.Update(glfwGetTime());
	const auto dt{timer.GetDelta()};
	cam->update(dt);
	// Window size changed.
	if (Input::GetInstance().ShouldResize()) {
		screen_width = Input::GetInstance().GetWidth();
		screen_height = Input::GetInstance().GetHeight();
		glViewport(0, 0, screen_width, screen_height);
	}
	Input::GetInstance().Update();
}
void Scene::handle_event()
{
	glfwPollEvents();
	process_input();
}
void Scene::render()
{
	glm::mat4 projection = GetProjMatrix();
	glm::mat4 view = GetViewMatrix();
	animation_model->Draw(0, view, projection);
}
void Scene::swap_buffers()
{
	glfwSwapBuffers(this->window.get());
}
void Scene::process_input()
{
	// Check if the window needs to be closed
	if (Input::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(this->window.get())) {
		glfwSetWindowShouldClose(this->window.get(), true);
	}
}
bool Scene::is_closing() const
{
	return glfwWindowShouldClose(this->window.get());
}
void Scene::attach_camera(const glm::vec3& Position, const glm::vec3& lookAt)
{
	cam = Camera::create(Position, lookAt);
}
