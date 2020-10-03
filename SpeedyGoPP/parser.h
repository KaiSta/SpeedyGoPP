#pragma once

#include <fstream>
#include <string>

#include "util.h"

class parser
{
public:
	parser(std::string path);
	bool getNext(Item& item);

private:
	std::ifstream infile;
};

