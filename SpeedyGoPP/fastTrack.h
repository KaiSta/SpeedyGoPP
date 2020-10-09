#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "util.h"
#include "register.h"

class fastTrack
{
public:
	fastTrack();
	fastTrack(const fastTrack&) = delete;
	void operator=(const fastTrack&) = delete;

	void put(const Item&);
private:
	void checkThreadInit(thread_t, VectorClock&);

	void read(const Item&);
	void write(const Item&);
	void lock(const Item&);
	void unlock(const Item&);
	void sig(const Item&);
	void wait(const Item&);
	void atomicOp(const Item&);

	std::unordered_map<thread_t, VectorClock> threads;
	std::unordered_map<obj_t, VectorClock> locks;
	std::unordered_map<obj_t, std::pair<VectorClock, VectorClock> > vars;//1 read 2 write
	std::unordered_map<obj_t, VectorClock> signals;
	std::unordered_map<obj_t, VectorClock> volatiles;

	std::function<void(std::string, std::string)> get_report();
};

namespace FastTrack
{
	class initializer {
	public:
		initializer()
		{
			auto& r = Register::getReg();
			std::vector<std::function<void(const Item&)> > funcs{std::bind(&fastTrack::put, &ft, std::placeholders::_1)};
			r.add("fastTrack", funcs);
		}
		initializer(const initializer&) = delete;
		initializer(initializer&&) = delete;
		void operator=(const initializer&) = delete;
		void operator=(initializer&&) = delete;
	private:
		fastTrack ft;
	};

	static const initializer ftInit;
}