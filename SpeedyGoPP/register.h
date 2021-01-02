#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>

#include "util.h"
#include "racedetector.h"

class Register {
public:	
	Register(const Register&) = delete;
	void operator=(Register const&) = delete;
	~Register();
	

	static Register& getReg();

	void setReporter(RaceDetector::reportFunc);
	RaceDetector::reportFunc getReporter();

	template <typename T>
	void add(std::string name) {
		algorithms[name] = std::make_unique<T>();
	}
	RaceDetector* get(std::string);

private:
	Register();
	std::map<std::string, std::unique_ptr<RaceDetector> > algorithms;
	RaceDetector::reportFunc report;
};