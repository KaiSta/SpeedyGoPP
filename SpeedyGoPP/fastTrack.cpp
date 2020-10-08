#include "fastTrack.h"

#include <iostream>

fastTrack::fastTrack()
{
}

void fastTrack::put(const Item& itm)
{
	switch (itm.op)
	{
	case OpType::Write:
		write(itm);
		break;
	case OpType::Read:
		read(itm);
		break;
	case OpType::Signal:
		sig(itm);
		break;
	case OpType::Wait:
		wait(itm);
		break;
	case OpType::Lock:
		lock(itm);
		break;
	case OpType::Unlock:
		unlock(itm);
		break;
	case OpType::Atomic_Read:
	case OpType::Atomic_Write:
		atomicOp(itm);
		break;
	default:
		std::cout << "unimplemented operation type\n";
	}
}

void fastTrack::read(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto var = vars[itm.objID];

	if (!var.second.less(thread)) //compare with write history
	{
		std::cout << "wr race!\n";
	}

	var.first.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}

void fastTrack::write(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto var = vars[itm.objID];

	if (!var.first.less(thread)) // compare with read history
	{
		std::cout << "rw race!\n";
	}
	if (!var.second.less(thread)) // compare with write history
	{
		std::cout << "ww race!\n";
	}

	var.second.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}

void fastTrack::lock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto lock = locks[itm.objID];

	thread.sync(lock);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void fastTrack::unlock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto lock = locks[itm.objID];

	lock.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	locks[itm.objID] = lock;
}

void fastTrack::sig(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto signal = signals[itm.objID];

	signal = thread;
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	signals[itm.objID] = signal;
}

void fastTrack::wait(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto signal = signals[itm.objID];

	thread.sync(signal);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void fastTrack::atomicOp(const Item& itm)
{
	auto thread = threads[itm.threadID];
	auto vol = volatiles[itm.objID];

	thread.sync(vol);
	vol.sync(thread);

	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	volatiles[itm.objID] = vol;
}
