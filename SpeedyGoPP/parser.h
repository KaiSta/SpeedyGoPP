#pragma once

#include <fstream>
#include <string>

#include "util.h"

class parser
{
public:
	parser(std::string path);
	Item getNext();

private:
	std::ifstream infile;
};

