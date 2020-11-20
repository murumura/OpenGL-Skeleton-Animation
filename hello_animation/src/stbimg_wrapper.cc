#define STB_IMAGE_IMPLEMENTATION
#include "stbimg_wrapper.h"

#include <iostream>

#include "stb_image.h"
STB_Image::STB_Image(std::string img_path, bool set_flip_vert_opt)
{
	fs::path p = fs::path(img_path);
	/** get absolute path */
	auto abs_path = fs::canonical(p);
	std::cout << "Image loaded: " << abs_path << std::endl;
	stbi_set_flip_vertically_on_load(set_flip_vert_opt);
	img_path_ = abs_path;
	allocate();
}
STB_Image::~STB_Image()
{
	stbi_image_free(img_data_);
}
void STB_Image::free_image_data()
{
	stbi_image_free(img_data_);
}
void STB_Image::allocate()
{
	img_data_ = stbi_load(img_path_.c_str(), &width_, &height_, &nrChannels_, 0);
	if (img_data_ == nullptr) {
		throw Error("Failed to load texture");
	}

	switch (nrChannels_) {
	case 1:
		format_ = GL_RED;
		break;
	case 3:
		format_ = GL_RGB;
		break;
	case 4:
		format_ = GL_RGBA;
		break;
	default:
		throw Error("Undefined Format!\n");
		break;
	}
}