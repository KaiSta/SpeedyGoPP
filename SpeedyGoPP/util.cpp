#include "util.h"

VectorClock::VectorClock() 
{
	vals.resize(4);
}

VectorClock::VectorClock(const VectorClock& vc) : vals{ vc.vals } 
{
}

VectorClock::~VectorClock() 
{

}

void VectorClock::add(thread_t tid, int val) 
{
	if (tid >= vals.size())
	{
		vals.resize(tid + 1);
	}
	set(tid, get(tid) + val);
}

void VectorClock::set(thread_t tid, int val) 
{
	if (tid >= vals.size()) 
	{
		vals.resize(tid + 1);
	}
	vals[tid] = val;
}

void VectorClock::sync(const VectorClock& vc) 
{
	for (std::vector<int>::size_type i = 0; i < vc.vals.size(); ++i)
	{
		set(i, max(get(i), vc.get(i)));
	}
}

bool VectorClock::less(const VectorClock& vc) const
{
	if (vals.empty())
	{
		return true;
	}

	bool oneRealSmaller = false;

	for (std::vector<int>::size_type i = 0; i < vals.size(); ++i)
	{
		if (get(i) > vc.get(i)) {
			return false;
		} 
		else if (get(i) < vc.get(i))
		{
			oneRealSmaller = true;
		}
	}

	if (vc.vals.size() > vals.size())
	{
		for (std::vector<int>::size_type i = 0; i < vc.vals.size(); ++i)
		{
			if (get(i) > vc.get(i))
			{
				return false;
			}
			else if (get(i) < vc.get(i))
			{
				oneRealSmaller = true;
			}
		}
	}

	return oneRealSmaller;
}

int VectorClock::get(thread_t tid) const
{
	if (tid > vals.size())
	{
		return 0;
	}
	return vals[tid];
}

int VectorClock::max(int a, int b) const
{
	if (a > b)
	{
		return a;
	}
	return b;
}

Reporter::Reporter(std::ostream& out, level detail, SrcRefManager& manager) : output(out), details(detail), all_races(0), unique_races(0), srcManager(manager)
{
}

void Reporter::race(int ai, int bi)
{
	auto a = srcManager.get(ai);
	auto b = srcManager.get(bi);
	++all_races;
	if (details == Reporter::level::ALL)
	{
		output << "Race: " << a << "," << b << std::endl;
		output << unique_races << "/" << all_races << std::endl;
	}

	if (!is_unique(a, b))
		return;

	++unique_races;

	auto innerMap = uniqueFilter[a];
	innerMap[b] = true;
	uniqueFilter[a] = innerMap;

	if (details == Reporter::level::UNIQUE)
	{
		output << "Race: " << a << "," << b << std::endl;
		output << unique_races << "/" << all_races << std::endl;
	}
}

bool Reporter::is_unique(std::string a, std::string b)
{
	auto innerMap = uniqueFilter.find(a);
	if (innerMap == uniqueFilter.end())
		return true;
	if (innerMap->second.find(b) == innerMap->second.end())
		return true;
	return false;
}

SrcRefManager::SrcRefManager() : counter(0)
{
}

int SrcRefManager::add(std::string s)
{
	auto res = s_to_i.find(s);
	if (res != s_to_i.end())
	{
		return res->second; // key already exists, return int val
	}
	//key does not exist, first idx starts with 1
	++counter;
	s_to_i[s] = counter;
	i_to_s[counter] = s;
	return counter;
}

std::string SrcRefManager::get(int i)
{
	if (i == NotAvailable)
		return "N/A";

	return i_to_s[i];
}
