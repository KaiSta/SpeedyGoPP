#pragma once

#include <vector>
#include <unordered_map>

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
};

