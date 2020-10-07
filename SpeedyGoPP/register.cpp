#include "register.h"

Register::Register()
{
}

Register::~Register()
{
}

void Register::add(std::string name, std::vector<EventListener> listeners)
{
	algorithms[name] = listeners;
}

std::vector<EventListener> Register::get(std::string name) const
{
	return algorithms[name]
}
