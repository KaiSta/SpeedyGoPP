#pragma once

#include <vector>
#include <string>

class VectorClock 
{
public:
	VectorClock();
	VectorClock(const VectorClock& vc);
	~VectorClock();
	void set(int tid, int val);
	void add(int tid, int val);
	void sync(const VectorClock& vcc);
	bool less(const VectorClock& vc) const;
	int get(int tid) const;
private:
	std::vector<int> vals;

	int max(int, int) const;
};

enum OpType
{
	Write,
	Read,
	Lock,
	Unlock,
	Atomic_Write,
	Atomic_Read,
	Signal,
	Wait,
	Fork,
	Join
};

struct Item
{
	int threadID;
	int partnerID;
	int objID;
	int idx;
	std::string sourceRef;
	OpType op;
};