#ifndef ARRAY_H
#define ARRAY_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
class Array {
   public:
	Array();
	~Array();
	/** To use a VAO all you have to do is bind the VAO using glBindVertexArray*/
	void bind()
	{
		glBindVertexArray(array_id);
	}
	void unbind()
	{
		glBindVertexArray(0);
	}
	GLuint id() const
	{
		return array_id;
	}
	static std::shared_ptr<Array> create()
	{
		return std::make_shared<Array>();
	}

   protected:
	GLuint array_id;
};
#endif