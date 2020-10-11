#include "fastTrack.h"

#include <iostream>

fastTrack::fastTrack()
{}

void fastTrack::setOutput(std::function<void(int,int)> fu)
{
	report = fu;
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

void fastTrack::checkThreadInit(thread_t tid, VectorClock& vc)
{
	if (vc.get(tid) == 0) //thread uninitialized
	{
		vc.set(tid, 1);
	}
}

void fastTrack::read(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);

	auto var = vars[itm.objID];

	if (!var.second.less(thread)) //compare with write history
	{
		report(itm.sourceRef, SrcRefManager::NotAvailable);
	}

	var.first.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}

void fastTrack::write(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto var = vars[itm.objID];

	if (!var.first.less(thread)) // compare with read history
	{
		report(itm.sourceRef, SrcRefManager::NotAvailable);
	}
	if (!var.second.less(thread)) // compare with write history
	{
		report(itm.sourceRef, SrcRefManager::NotAvailable);
	}

	var.second.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}

void fastTrack::lock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto lock = locks[itm.objID];

	thread.sync(lock);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void fastTrack::unlock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto lock = locks[itm.objID];

	lock.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	locks[itm.objID] = lock;
}

void fastTrack::sig(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto signal = signals[itm.objID];

	signal = thread;
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	signals[itm.objID] = signal;
}

void fastTrack::wait(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto signal = signals[itm.objID];

	thread.sync(signal);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void fastTrack::atomicOp(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto vol = volatiles[itm.objID];

	thread.sync(vol);
	vol.sync(thread);

	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	volatiles[itm.objID] = vol;
}

std::function<void(int,int)> fastTrack::get_report()
{
	auto& r = Register::getReg();
	return r.getReporter();
}
