#include "parser.h"

parser::parser(std::string path, SrcRefManager& manager) : infile(path), count(0), srcManager(manager)
{
}

parser::~parser()
{
	infile.close();
}

bool parser::getNext(Item& item)
{
	std::string line;

	if (std::getline(infile, line))
	{
		size_t pos = 0;
		std::vector<std::string> tokens;
		while ((pos = line.find(delimiter)) != std::string::npos) 
		{
			tokens.push_back(line.substr(0, pos));		
			line.erase(0, pos + delimiter.length());
		}

		item.threadID = thread_t(std::stoi(tokens[0]));
		item.objID = obj_t(std::stoi(tokens[2]));
		item.idx = count;

		//auto res = sourceRefs.insert({ tokens[3], true });
		item.sourceRef = srcManager.add(tokens[3]);
		
		if (tokens[1] == "WR") 
		{
			item.op = OpType::Write;
		}
		else if (tokens[1] == "RD") 
		{
			item.op = OpType::Read;
		}
		else if (tokens[1] == "LK")
		{
			item.op = OpType::Lock;
		}
		else if (tokens[1] == "UK")
		{
			item.op = OpType::Unlock;
		}
		else if (tokens[1] == "SIG")
		{
			item.op = OpType::Signal;
		}
		else if (tokens[1] == "WT")
		{
			item.op = OpType::Wait;
		}
		else if (tokens[1] == "AWR")
		{
			item.op = OpType::Atomic_Write;
		}
		else if (tokens[1] == "ARD")
		{
			item.op = OpType::Atomic_Read;
		}
		return true;
	} 

	return false;
}
