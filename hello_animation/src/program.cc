#include "program.h"
Program::Program()
{
	allocate();
}
Program::~Program()
{
	glDeleteProgram(program_id);
}
void Program::allocate()
{
	program_id = glCreateProgram();
	if (program_id == 0u) {
		throw Error("Failed to allocate an OpenGL program");
	}
}
void Program::link() const
{
	glLinkProgram(program_id);
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

	if (link_status != GL_TRUE) {
		std::string message = "Failed to link program";
		GLint log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			auto log = std::make_unique<char[]>(log_length);
			glGetProgramInfoLog(program_id, log_length, &log_length, log.get());
			message += "\n";
			message += log.get();
			message += "\n";
		}
		throw Error(message);
	}
}
