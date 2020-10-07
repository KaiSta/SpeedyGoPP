#pragma once

#include <fstream>
#include <string>

#include "util.h"

class parser
{
public:
	parser(std::string path);
	parser(const parser&) = delete;
	~parser();
	bool getNext(Item& item);

private:
	std::ifstream infile;
	int count;

	const std::string delimiter = ",";
};

