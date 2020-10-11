#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>

#include "util.h"

class Register {
public:
	typedef std::function<void(int,int)> reportFunc;
	
	Register(const Register&) = delete;
	void operator=(Register const&) = delete;
	~Register();
	

	static Register& getReg();

	void setReporter(reportFunc);
	reportFunc getReporter();


	void add(std::string, std::vector<std::function<void(const Item&)> >, std::function<void(reportFunc)>);
	const std::vector<std::function<void(const Item&)> >& get(std::string);

private:
	Register();
	std::map<std::string, std::vector<std::function<void(const Item&)> > > algorithms;
	reportFunc report;

	std::vector< std::function<void(reportFunc)> > cbReports;
};