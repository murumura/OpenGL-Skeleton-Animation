#ifndef ERR_H
#define ERR_H
#include <stdexcept>
class Error : public std::runtime_error {
   public:
	Error(const std::string& msg) : std::runtime_error(msg){};
};
#endif