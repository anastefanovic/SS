#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

namespace utils
{
	static int stoi(const std::string& str)
	{
		return std::stoi(str, nullptr, 0);
	}
}


#endif