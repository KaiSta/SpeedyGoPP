#include "register.h"

Register::Register()
{
}

Register::~Register()
{
}

Register& Register::getReg()
{
	static Register lreg;
	return lreg;
}

void Register::add(std::string name, std::vector<EventListener> listeners)
{
	algorithms[name] = listeners;
}

std::vector<EventListener> Register::get(std::string name)
{
	return algorithms[name];
}

void EventListener::put(const Item&)
{
}
