#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <filesystem>
#include <map>
#include <string>
#include <vector>
namespace fs = std::filesystem;
class TextureManager {
   public:
	static uint32_t LoadTexture(std::string path);
};
#endif