#ifndef BUFFER_H
#define BUFFER_H
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>

#include "err.h"
#include "typedef.h"
class Buffer {
   public:
	Buffer();
	~Buffer();
	void set_buffer_data(GLenum target, GLenum usage, const void* ptr, size_t size);
	void set_buffer_data(GLenum target, GLenum usage, std::initializer_list<GLfloat> data)
	{
		set_buffer_data(target, usage, &*data.begin(),
		                (uintptr_t) & *data.end() - (uintptr_t) & *data.begin());
	}

	void set_buffer_data_d(GLenum target, GLenum usage, std::initializer_list<GLdouble> data)
	{
		set_buffer_data(target, usage, &*data.begin(),
		                (uintptr_t) & *data.end() - (uintptr_t) & *data.begin());
	}

	void set_buffer_data(GLenum target, GLenum usage, std::vector<Vertex>& data)
	{
		buffer_target = target;
		bind();
		glBufferData(target, data.size() * sizeof(Vertex), &data[0], usage);
	}
	void set_buffer_data(GLenum target, GLenum usage, std::vector<uint32_t>& data)
	{
		buffer_target = target;
		bind();
		glBufferData(target, data.size() * sizeof(uint32_t), &data[0], usage);
	}
	GLuint id() const { return buffer_id; }
	void bind() const { glBindBuffer(buffer_target, buffer_id); }
	void unbind() const { glBindBuffer(buffer_target, 0); }
	static std::shared_ptr<Buffer>
	create()
	{
		return std::make_shared<Buffer>();
	}

	static std::shared_ptr<Buffer>
	create(GLenum target, GLenum usage, const void* ptr, size_t size)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->set_buffer_data(target, usage, ptr, size);
		return buffer;
	}

	static std::shared_ptr<Buffer>
	create(GLenum target, GLenum usage,
	       std::initializer_list<GLfloat> data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->set_buffer_data(target, usage, data);
		return buffer;
	}
	static std::shared_ptr<Buffer>
	create(GLenum target, GLenum usage,
	       std::vector<Vertex>& data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->set_buffer_data(target, usage, data);
		return buffer;
	}
	static std::shared_ptr<Buffer>
	create(GLenum target, GLenum usage,
	       std::vector<uint32_t>& data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->set_buffer_data(target, usage, data);
		return buffer;
	}
	static std::shared_ptr<Buffer>
	create_d(GLenum target, GLenum usage,
	         std::initializer_list<GLdouble> data)
	{
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->set_buffer_data_d(target, usage, data);
		return buffer;
	}

   protected:
	GLuint buffer_id;
	GLenum buffer_target;
	void allocate();
};

#endif