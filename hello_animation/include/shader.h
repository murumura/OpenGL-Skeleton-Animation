#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <string>

#include "err.h"

class Shader {
   public:
	Shader(GLenum shader_type);
	~Shader();
	void set_source(const std::string src, int length = -1);
	/* Load shader source from file */
	void set_source_from_file(const std::string filename);

	GLuint id() const { return shader_id_; }
	std::string name_for_type()
	{
		switch (shader_type_) {
		case GL_VERTEX_SHADER:
			return "Vertex";
		case GL_FRAGMENT_SHADER:
			return "Fragment";
		default:
			return "Unknown";
			break;
		}
	}

	static std::shared_ptr<Shader>
	create(GLenum shader_type, const std::string src, int length = -1)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shader_type);
		shader->set_source(src, length);
		return shader;
	}

	static std::shared_ptr<Shader>
	create_from_file(GLenum shader_type, const std::string filename)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(shader_type);
		shader->set_source_from_file(filename);
		return shader;
	}

   protected:
	/* The shader object's ID */
	GLuint shader_id_;
	/* The OpenGL shader type */
	GLenum shader_type_;
	/* Allocate the OpenGL shader object. */
	void allocate();
};
#endif