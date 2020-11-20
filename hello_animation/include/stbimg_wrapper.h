#ifndef _STB_IMG_H
#define _STB_IMG_H
#include <glad/glad.h>

#include <filesystem>
#include <iostream>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "err.h"
namespace fs = std::filesystem;
class STB_Image {
   public:
	STB_Image(std::string img_path, bool set_flip_vert_opt = true);
	~STB_Image();
	void free_image_data();
	inline unsigned char* img_data()
	{
		return img_data_;
	}
	inline auto width()
	{
		return width_;
	}
	inline auto height()
	{
		return height_;
	}
	inline auto channel()
	{
		return nrChannels_;
	}
	inline auto format()
	{
		return format_;
	}
	static std::shared_ptr<STB_Image>
	create(std::string img_path)
	{
		std::shared_ptr<STB_Image> stb_img_ptr = std::make_shared<STB_Image>(img_path);
		return stb_img_ptr;
	}
	void allocate();

   protected:
	int width_, height_, nrChannels_;
	GLenum format_;
	std::string img_path_;
	unsigned char* img_data_;
};
#endif