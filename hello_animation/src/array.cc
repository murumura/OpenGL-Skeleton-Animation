#include "array.h"

#include "err.h"
Array::Array()
{
	array_id = 0u;
	/**glGenVertexArrays gen a VAO with output id:array_id */
	glGenVertexArrays(1, &array_id);
	if (array_id == 0u) {
		throw Error("Failed to allocate an OpenGL array");
	}
}
Array::~Array()
{
	glDeleteVertexArrays(1, &array_id);
}
