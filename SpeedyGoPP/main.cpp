#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "parser.h"

enum class argType {
	NONE,
	MODE,
	TRACE
};

int main(int argc, char* argv[]) {

	std::string modus;
	std::string tracePath;

	auto state = argType::NONE;
	for (int i = 0; i < argc; ++i) {
		if (state == argType::NONE)
		{
			if (std::string(argv[i]) == "-mode")
			{
				state = argType::MODE;
			}
			else if (std::string(argv[i]) == "-trace")
			{
				state = argType::TRACE;
			}
		}
		else if (state == argType::MODE)
		{
			modus = std::string(argv[i]);
			state = argType::NONE;
		}
		else if (state == argType::TRACE)
		{
			tracePath = std::string(argv[i]);
			state = argType::NONE;
		}
	}

	std::cout << "Mode=" << modus << std::endl;
	std::cout << "Trace=" << tracePath << std::endl;

	parser p{ tracePath };

	Item it;
	while (p.getNext(it))
	{
		std::cout << "{" << std::endl;
		std::cout << "\tTID:" << it.threadID << std::endl;
		switch (it.op)
		{
		case OpType::Write:
			std::cout << "\tTYPE:" << "write" << std::endl;
			break;
		case OpType::Read:
			std::cout << "\tTYPE:" << "read" << std::endl;
			break;
		case OpType::Lock:
			std::cout << "\tTYPE:" << "lock" << std::endl;
			break;
		case OpType::Unlock:
			std::cout << "\tTYPE:" << "unlock" << std::endl;
			break;
		case OpType::Signal:
			std::cout << "\tTYPE:" << "signal" << std::endl;
			break;
		case OpType::Wait:
			std::cout << "\tTYPE:" << "wait" << std::endl;
			break;
		default:
			std::cout << "\tTYPE:" << "unknown" << std::endl;
		}
		
		std::cout << "\tOBJ:" << it.objID << std::endl;
		std::cout << "\tSRCREF:" << it.sourceRef << std::endl;
		std::cout << "}" << std::endl;
	}

	return 0;
}