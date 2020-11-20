#include "buffer.h"

#include <iostream>
Buffer::Buffer()
{
	allocate();
}
Buffer::~Buffer()
{
	glDeleteBuffers(1, &buffer_id);
}
void Buffer::set_buffer_data(GLenum target, GLenum usage, const void* ptr, size_t size)
{
	/* call to the glBufferData function that copies the previously defined vertex data into the buffer's memory */
	buffer_target = target;
	bind();
	glBufferData(target, size, ptr, usage);
}
void Buffer::allocate()
{
	/** generate buffer object */
	buffer_id = 0u;
	glGenBuffers(1, &buffer_id);
	if (buffer_id == 0u) {
		throw Error("Failed to allocate an OpenGL buffer");
	}
}