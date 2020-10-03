#include "parser.h"

parser::parser(std::string path) : infile(path)
{
}

bool parser::getNext(Item& item)
{
	std::string line;

	if (std::getline(infile, line))
	{
		return true;
	} 

	return false;
}
