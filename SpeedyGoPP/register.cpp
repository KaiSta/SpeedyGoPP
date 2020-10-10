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

void Register::setReporter(reportFunc f)
{
	for (auto& e : cbReports)
	{
		e(f);
	}
}

Register::reportFunc Register::getReporter()
{
	return report;
}

void Register::add(std::string name, std::vector<std::function<void(const Item&)> > listeners, std::function<void(reportFunc)> cbReporter)
{
	algorithms[name] = listeners;
	cbReports.push_back(cbReporter);
}

const std::vector<std::function<void(const Item&)> >& Register::get(std::string name)
{
	return algorithms[name];
}
