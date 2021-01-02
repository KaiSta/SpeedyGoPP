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

void Register::setReporter(RaceDetector::reportFunc f)
{
	report = f;
}

RaceDetector::reportFunc Register::getReporter()
{
	return report;
}

RaceDetector* Register::get(std::string name)
{
	algorithms[name]->setReporter(report);
	return algorithms[name].get();
}
