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

void Register::add(std::string name, std::vector<std::function<void(const Item&)> > listeners)
{
	algorithms[name] = listeners;
}

const std::vector<std::function<void(const Item&)> >& Register::get(std::string name)
{
	return algorithms[name];
}
