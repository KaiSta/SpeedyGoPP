#include "tsanv1.h"

void checkThreadInit(thread_t tid, VectorClock& vc)
{
	if (vc.get(tid) == 0) //thread uninitialized
	{
		vc.set(tid, 1);
	}
}
    
ThreadSanitizer::~ThreadSanitizer()
{}

void ThreadSanitizer::read(const Item& itm)
{
    auto thread = threads[itm.threadID];
    auto var = vars[itm.objID];

    std::vector<access> readTmp;

    for (auto& e : var.first) //reads
    {
        if (!e.vc.less(thread.first))
        {
            readTmp.push_back(e);
        }
    }
    readTmp.push_back(access{thread.first, thread.second, itm});
    var.first = readTmp;

    for (auto& e : var.second) //writes
    {
        if (!e.vc.less(thread.first) && !thread.second.containsCommon(e.ls))
        {
            report(itm.sourceRef, e.itm.sourceRef);
        }
    }

    vars[itm.objID] = var;
}

void ThreadSanitizer::write(const Item& itm)
{
    auto thread = threads[itm.threadID];
    auto var = vars[itm.objID];

    std::vector<access> writeTmp;

    for (auto& e : var.second) //writes
    {
        if (e.vc.less(thread.first))
        {
            continue;
        }
        writeTmp.push_back(e);
        if (!thread.second.containsCommon(e.ls))
        {
            report(itm.sourceRef, e.itm.sourceRef);
        }
    }
    writeTmp.push_back(access{thread.first, thread.second, itm});
    var.second = writeTmp;

    for (auto& e : var.first) //writes
    {
        if (!e.vc.less(thread.first) && !thread.second.containsCommon(e.ls))
        {
            report(itm.sourceRef, e.itm.sourceRef);
        }
    }

    vars[itm.objID] = var;
}

void ThreadSanitizer::lock(const Item& itm)
{
    auto thread = threads[itm.threadID];
    thread.second.add(itm.objID);
    threads[itm.threadID] = thread;
}

void ThreadSanitizer::unlock(const Item& itm)
{
    auto thread = threads[itm.threadID];
    thread.second.remove(itm.objID);
    threads[itm.threadID] = thread;
}

void ThreadSanitizer::signal(const Item& itm)
{
    auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread.first);
	auto signal = signals[itm.objID];

	signal = thread.first;
	thread.first.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	signals[itm.objID] = signal;
}

void ThreadSanitizer::wait(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread.first);
	auto signal = signals[itm.objID];

	thread.first.sync(signal);
	thread.first.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
}

void ThreadSanitizer::atomicOp(const Item& itm)
{
	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread.first);
	auto vol = volatiles[itm.objID];

	thread.first.sync(vol);
	vol.sync(thread.first);

	thread.first.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	volatiles[itm.objID] = vol;
}
