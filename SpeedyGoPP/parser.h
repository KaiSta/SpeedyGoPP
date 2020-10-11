#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "util.h"

class parser
{
public:
	parser(std::string path, SrcRefManager&);
	parser(const parser&) = delete;
	~parser();
	parser& operator=(const parser&) = delete;
	bool getNext(Item& item);

private:
	std::ifstream infile;
	const std::string delimiter = ",";
	size_t count;

	SrcRefManager& srcManager;
};

/*
put each sourceref in a map, give each item just the ref to the sourceref
*/