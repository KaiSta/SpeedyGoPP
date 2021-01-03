#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <ostream>
#include <algorithm>

typedef std::vector<int>::size_type thread_t;
typedef std::vector<int>::size_type obj_t;

class VectorClock 
{
public:
	VectorClock();
	VectorClock(thread_t tid);
	VectorClock(const VectorClock& vc);
	~VectorClock();
	void set(thread_t tid, int val);
	void add(thread_t tid, int val);
	void sync(const VectorClock& vcc);
	bool less(const VectorClock& vc) const;
	int get(thread_t tid) const;

	friend std::ostream& operator<< (std::ostream& stream, const VectorClock& vc);
private:
	std::vector<int> vals;
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
	int sourceRef;
	thread_t threadID{};
	thread_t partnerID{};
	obj_t objID{};
	size_t idx{};
};

class SrcRefManager
{
public:
	static const int NotAvailable = 0;

	SrcRefManager();
	SrcRefManager(const SrcRefManager&) = delete;
	void operator=(const SrcRefManager&) = delete;

	int add(std::string);
	std::string get(int);

private:
	std::unordered_map<std::string, int> s_to_i;
	std::unordered_map<int, std::string> i_to_s;

	int counter;
};

class Reporter
{
public:
	enum class level : size_t
	{
		ALL,
		UNIQUE
	};

	Reporter(std::ostream&, level, SrcRefManager&);
	void race(int, int);

	Reporter(const Reporter&) = delete;
	Reporter(Reporter&&) = delete;
	void operator=(const Reporter&) = delete;
private:
	std::ostream& output;
	level details;
	std::unordered_map<std::string, std::unordered_map<std::string, bool> > uniqueFilter;
	size_t all_races;
	size_t unique_races;

	bool is_unique(std::string, std::string);

	SrcRefManager& srcManager;
};

class Lockset {
public:
	Lockset()=default;
	Lockset(const Lockset&);
	Lockset& operator=(Lockset ls);

	void add(obj_t lock);
	void remove(obj_t lock);
	Lockset intersect(const Lockset& lockset);
	bool containsCommon(const Lockset& lockset);
private:
	std::unordered_map<obj_t, bool> locks;
};