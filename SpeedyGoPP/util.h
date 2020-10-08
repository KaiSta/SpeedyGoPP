#pragma once

#include <vector>
#include <string>

typedef std::vector<int>::size_type thread_t;
typedef std::vector<int>::size_type obj_t;

class VectorClock 
{
public:
	VectorClock();
	VectorClock(const VectorClock& vc);
	~VectorClock();
	void set(thread_t tid, int val);
	void add(thread_t tid, int val);
	void sync(const VectorClock& vcc);
	bool less(const VectorClock& vc) const;
	int get(thread_t tid) const;
private:
	std::vector<int> vals;

	int max(int, int) const;
};

enum class OpType : size_t
{
	Write,
	Read,
	Lock,
	Unlock,
	Atomic_Write,
	Atomic_Read,
	Signal,
	Wait,
};

struct Item
{
	OpType op{};
	std::string sourceRef{};
	thread_t threadID{};
	thread_t partnerID{};
	obj_t objID{};
	size_t idx{};
};