#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>

#include "util.h"

class EventListener {
public:
	virtual void put(const Item&)
	{}
	virtual ~EventListener() {}
};

class Register {
public:
	
	Register(const Register&) = delete;
	void operator=(Register const&) = delete;
	~Register();
	

	static Register& getReg();

	void add(std::string, std::vector<std::function<void(const Item&)> >);
	const std::vector<std::function<void(const Item&)> >& get(std::string);

private:
	Register();
	std::map<std::string, std::vector<std::function<void(const Item&)> > > algorithms;
};