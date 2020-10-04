#include "defines.h"

template<typename T>
T EnumParser<T>::Parse(const std::string& name)
{
	auto it = map.find(name);
	if (it = map.end())
	{
		return NULL;
	}
	return (*it).second;
}
