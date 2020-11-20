#ifndef UTIL_H
#define UTIL_H
#include <map>
#include <string>
class Bone_Namer {
   public:
	Bone_Namer()
	{
		clear();
	}
	void clear()
	{
		map_.clear();
		total_ = 0;
	}
	uint32_t Name(const std::string &name)
	{
		if (map_.count(name))
			return map_[name];
		return map_[name] = total_++;
	}
	uint32_t total() const
	{
		return total_;
	}
	std::map<std::string, uint32_t> &map()
	{
		return map_;
	}

   private:
	std::map<std::string, uint32_t> map_;
	uint32_t total_;
};
#endif