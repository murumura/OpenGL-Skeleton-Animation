#include "texture_manager.h"

#include <glad/glad.h>

#include "err.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "stb_image.h"
uint32_t TextureManager::LoadTexture(std::string path)
{
	uint32_t texture;
	static std::map<std::string, uint32_t> memory;
	if (memory.count(path.c_str())) return memory[path.c_str()];
	int w, h, comp;
	unsigned char* image = stbi_load(path.c_str(), &w, &h, &comp, 0);
	if (image == nullptr) throw Error("Load picture error");

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::vector<float> border_color = {0.0, 0.0, 0.0, 0.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color.data());

	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);
	return memory[path.c_str()] = texture;
}
