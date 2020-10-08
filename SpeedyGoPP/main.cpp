#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "parser.h"
#include "register.h"
#include "util.h"

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

	auto& reg = Register::getReg();
	auto algorithm = reg.get(modus);

	if (algorithm.empty())
	{
		std::cout << "Unknown algorithm" << std::endl;
		return 1;
	}

	Item it;
	while (p.getNext(it))
	{
		for (auto& listener : algorithm)
		{
			listener.put(it);
		}
		it = {}; //reset item
	}

	return 0;
}