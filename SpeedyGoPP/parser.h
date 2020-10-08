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
	parser& operator=(const parser&) = delete;
	bool getNext(Item& item);

private:
	std::ifstream infile;
	const std::string delimiter = ",";
	size_t count;
};

