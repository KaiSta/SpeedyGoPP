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
	Register();
	~Register();
	Register(const Register&) = delete;

	void add(std::string, std::vector<EventListener>);
	std::vector<EventListener> get(std::string);

private:
	std::map<std::string, std::vector<EventListener> > algorithms;
};