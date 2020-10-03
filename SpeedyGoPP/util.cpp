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

void VectorClock::add(int tid, int val) 
{
	if (tid >= vals.size())
	{
		vals.resize(tid + 1);
	}
	set(tid, get(tid) + val);
}

void VectorClock::set(int tid, int val) 
{
	if (tid >= vals.size()) 
	{
		vals.resize(tid + 1);
	}
	vals[tid] = val;
}

void VectorClock::sync(const VectorClock& vc) 
{
	for (int i = 0; i < vc.vals.size(); ++i)
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

	for (int i = 0; i < vals.size(); ++i)
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
		for (int i = 0; i < vc.vals.size(); ++i)
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

int VectorClock::get(int tid) const
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
