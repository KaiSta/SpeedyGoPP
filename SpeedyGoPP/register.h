#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>

#include "util.h"

class Register {
public:
	typedef std::function<void(std::string, std::string)> reportFunc;
	
	Register(const Register&) = delete;
	void operator=(Register const&) = delete;
	~Register();
	

	static Register& getReg();

	void setReporter(reportFunc);
	reportFunc getReporter();


	void add(std::string, std::vector<std::function<void(const Item&)> >);
	const std::vector<std::function<void(const Item&)> >& get(std::string);

private:
	Register();
	std::map<std::string, std::vector<std::function<void(const Item&)> > > algorithms;
	reportFunc report;
};