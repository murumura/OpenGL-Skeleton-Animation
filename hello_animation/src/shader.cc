#include "shader.h"

#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "err.h"
namespace fs = std::filesystem;
Shader::Shader(GLenum shader_type)
{
	shader_type_ = shader_type;
	allocate();
}
Shader::~Shader()
{
	glDeleteShader(shader_id_);
}
void Shader::set_source(const std::string src, int length)
{
	GLint status;
	GLint lengths[1] = {length};
	const char *c_str = src.c_str();
	/* glShaderSource — Replaces the source code in a shader object */
	glShaderSource(shader_id_, 1, &c_str, &lengths[0]);
	glCompileShader(shader_id_);
	/** query a shader for compile information*/
	glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		return;
	}

	std::string err_message = "Error compiling ";
	switch (shader_type_) {
	case GL_VERTEX_SHADER:
		err_message += "vertex";
		break;
	case GL_FRAGMENT_SHADER:
		err_message += "fragment";
		break;
	default:
		err_message += "unknown";
		break;
	}
	err_message += " shader";

	GLint log_length;
	glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 0) {
		auto log = std::make_unique<char[]>(log_length);

		/**return information log contain shader object complication*/
		glGetShaderInfoLog(shader_id_, log_length, &log_length, log.get());
		err_message += "\nShader Log:\n";
		err_message += log.get();
		err_message += "\n";
	}
	glDeleteShader(shader_id_);
	throw Error(err_message);
}
void Shader::set_source_from_file(const std::string filename)
{
	// Open the stream to 'lock' the file.
	std::ifstream f(filename, std::ios::in | std::ios::binary);

	// Obtain the size of the file.
	const auto file_len = fs::file_size(filename);

	// Create a buffer.
	std::string file_content(file_len, '\0');

	// Read the whole file into the buffer.
	f.read(file_content.data(), file_len);

	set_source(file_content, file_len);
}
void Shader::allocate()
{
	shader_id_ = glCreateShader(shader_type_);
	if (shader_id_ == 0u) {
		throw Error("Failed to allocate an OpenGL shader");
	}
}
