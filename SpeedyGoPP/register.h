#pragma once
#include <string>
#include <map>

#include "util.h"

class EventListener {
public:
	void put(const Item&);
};

class Register {
public:
	
	Register(const Register&) = delete;
	void operator=(Register const&) = delete;
	~Register();
	

	static Register& getReg();

	void add(std::string, std::vector<EventListener>);
	std::vector<EventListener> get(std::string);

private:
	Register();
	std::map<std::string, std::vector<EventListener> > algorithms;
};