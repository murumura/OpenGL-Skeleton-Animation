#ifndef PROGRAM_H
#define PROGRAM_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <initializer_list>
#include <memory>
#include <string>

#include "err.h"
#include "shader.h"
#include "typedef.h"
class Program {
   public:
	Program();
	~Program();
	/** Utility method to attach shaders and link the program.
      *
      * This method automatically invokes attach_shader() for each
      * shader given and then invokes link() to link the program.
      *
      * @param shaders
      *   A std::unique_ptr<Shader>, std::shared_ptr<Shader>, or
      *   const Shader &.
    */
	template <typename... Shaders>
	void build(Shaders... shaders)
	{
		_build(shaders...);
	}
	/** Attach a shader to the program.*/
	void attach_shader(std::shared_ptr<Shader> shader) const
	{
		glAttachShader(program_id, shader->id());
	}
	void attach_shader(std::unique_ptr<Shader> shader) const
	{
		glAttachShader(program_id, shader->id());
	}
	void attach_shader(const Shader& shader) const
	{
		glAttachShader(program_id, shader.id());
	}
	void link() const;
	/*Get the index of a uniform variable in the program.*/
	GLint get_uniform_location(const char* uniform_name)
	{
		return glGetUniformLocation(program_id, uniform_name);
	}
	/*Factory method to construct a program object*/
	template <typename... Args>
	static std::shared_ptr<Program>
	create(Args... args)
	{
		std::shared_ptr<Program> program = std::make_shared<Program>();
		program->build(args...);
		return program;
	}
	void use() const
	{
		glUseProgram(program_id);
	}
	void set_uniform(std::string const& name, const glm::mat4& mat)
	{
		GLint location = get_uniform_location(name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}
	void set_uniform(std::string const& name, bool value)
	{
		GLint location = get_uniform_location(name.c_str());
		glUniform1i(location, (int)value);
	}
	// ------------------------------------------------------------------------
	void set_uniform(std::string const& name, int value)
	{
		GLint location = get_uniform_location(name.c_str());
		glUniform1i(location, value);
	}
	void set_uniform(std::string const& name, const std::vector<glm::mat4>& value)
	{
		GLint location = get_uniform_location(name.c_str());
		glUniformMatrix4fv(location, value.size(), GL_FALSE, glm::value_ptr(value[0]));
	}

   protected:
	GLuint program_id;
	void allocate();
	void _build() const
	{
		link();
	}
	/*internal helper function for different type of constructor*/
	template <typename... Shaders>
	void _build(const Shader& shader, Shaders... args) const
	{
		attach_shader(shader);
		_build(args...);
	}
	template <typename... Shaders>
	void _build(std::unique_ptr<Shader> shader, Shaders... args) const
	{
		attach_shader(shader);
		_build(args...);
	}
	template <typename... Shaders>
	void _build(std::shared_ptr<Shader> shader, Shaders... args) const
	{
		attach_shader(shader);
		_build(args...);
	}
	template <typename... Shaders>
	void _build(const char* attr_name, GLuint index, Shaders... args) const
	{
		glBindAttribLocation(program_id, index, attr_name);
		_build(args...);
	}
};
#endif