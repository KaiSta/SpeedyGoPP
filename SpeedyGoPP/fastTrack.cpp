#include "fastTrack.h"

#include <iostream>

FastTrack::~FastTrack() {}

void FastTrack::checkThreadInit(thread_t tid, VectorClock& vc)
{
	if (vc.get(tid) == 0) //thread uninitialized
	{
		vc.set(tid, 1);
	}
}

void FastTrack::read(const Item& itm)
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

void FastTrack::write(const Item& itm)
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

void FastTrack::lock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto lock = locks[itm.objID];

	thread.sync(lock);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void FastTrack::unlock(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto lock = locks[itm.objID];

	lock.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	locks[itm.objID] = lock;
}

void FastTrack::signal(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto signal = signals[itm.objID];

	signal = thread;
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	signals[itm.objID] = signal;
}

void FastTrack::wait(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);
	auto signal = signals[itm.objID];

	thread.sync(signal);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void FastTrack::atomicOp(const Item& itm)
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
