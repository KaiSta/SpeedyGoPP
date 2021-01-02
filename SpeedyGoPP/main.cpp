#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "parser.h"
#include "register.h"
#include "util.h"

#include "fastTrack.h"
#include "SHB.h"

enum class argType {
	NONE,
	MODE,
	TRACE
};

void registerAlgorithms(Register& reg)
{
	reg.add<FastTrack>("fastTrack");
	reg.add<SHB>("SHB");
}

int main(int argc, char* argv[]) 
{
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

	SrcRefManager srcManager;

	parser p{ tracePath, srcManager };

	auto& reg = Register::getReg();
	Reporter r {std::cout, Reporter::level::UNIQUE, srcManager};
	reg.setReporter(std::bind(&Reporter::race, &r, std::placeholders::_1, std::placeholders::_2));
	
	registerAlgorithms(reg);
	
	RaceDetector* alg = reg.get(modus);

	Item it;
	while (p.getNext(it))
	{
		switch (it.op)
		{
		case OpType::Write:
			alg->write(it);
			break;
		case OpType::Read:
			alg->read(it);
			break;
		case OpType::Signal:
			alg->signal(it);
			break;
		case OpType::Wait:
			alg->wait(it);
			break;
		case OpType::Lock:
			alg->lock(it);
			break;
		case OpType::Unlock:
			alg->unlock(it);
			break;
		case OpType::Atomic_Read:
		case OpType::Atomic_Write:
			alg->atomicOp(it);
			break;
		default:
			std::cout << "unimplemented operation type\n";
		}

		it = {}; //reset item
	}

	return 0;
}