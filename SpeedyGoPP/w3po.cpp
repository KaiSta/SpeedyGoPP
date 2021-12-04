#include "w3po.h"

W3POBase::~W3POBase() {}

void W3POBase::checkThreadInit(thread_t tid, VectorClock& vc)
{
	if (vc.get(tid) == 0) //thread uninitialized
	{
		vc.set(tid, 1);
	}
}

void W3POBase::read(const Item& itm)
{
    auto thread = threads[itm.threadID];
    checkThreadInit(thread.id, thread.tVc);

    auto var = variables[itm.objID];
    ++var.current;

    auto newFe = Dot {var.current, thread.tVc, itm.sourceRef, itm.threadID, false, thread.tLockset};

    std::vector<Dot> newFrontier;
    for (auto& dot : var.frontier)
    {
        auto k = dot.vc.get(dot.tid);
        auto thi_at_j = thread.tVc.get(dot.tid);

        if (k > thi_at_j)
        {
            newFrontier.push_back(std::move(dot));
            if (dot.write && !thread.tLockset.containsCommon(dot.ls))
            {
                report(itm.sourceRef, dot.sourceRef);
            }
        }
        else
        {
            if (dot.write)
            {
                newFrontier.push_back(std::move(dot));
            }
        }
    }
    newFrontier.push_back(newFe);
    var.frontier = newFrontier;

    thread.tVc.add(thread.id, 1);

    threads[itm.threadID] = thread;
    variables[itm.objID] = var;
}

void W3POBase::write(const Item& itm)
{
    auto thread = threads[itm.threadID];
    checkThreadInit(thread.id, thread.tVc);

    auto var = variables[itm.objID];
    ++var.current;

    auto newFe = Dot {var.current, thread.tVc, itm.sourceRef, itm.threadID, false, thread.tLockset};

    std::vector<Dot> newFrontier;
    for (auto& dot : var.frontier)
    {
        auto k = dot.vc.get(dot.tid);
        auto thi_at_j = thread.tVc.get(dot.tid);

        if (k > thi_at_j)
        {
            newFrontier.push_back(std::move(dot));
            if (!thread.tLockset.containsCommon(dot.ls))
            {
                report(itm.sourceRef, dot.sourceRef);
            }
        }
    }
    newFrontier.push_back(newFe);
    var.frontier = newFrontier;

    thread.tVc.add(thread.id, 1);

    threads[itm.threadID] = thread;
    variables[itm.objID] = var;
}

void W3POBase::lock(const Item& itm)
{
    auto thread = threads[itm.threadID];
    auto lock = locks[itm.objID];

    thread.tLockset.add(itm.objID);
    thread.tVc.add(itm.threadID, 1);
    
    threads[itm.threadID] = thread;
    locks[itm.objID] = lock;
}

void W3POBase::unlock(const Item& itm)
{
    auto thread = threads[itm.threadID];
    auto lock = locks[itm.objID];
    thread.tLockset.remove(itm.objID);
    thread.tVc.add(thread.id, 1);
    threads[itm.threadID] = thread;
    locks[itm.objID] = lock;
}

void W3POBase::signal(const Item& itm)
{
    auto thread = threads[itm.threadID];
    checkThreadInit(thread.id, thread.tVc);
    auto signal = signals[itm.objID];
    signal = thread.tVc;
    thread.tVc.add(thread.id, 1);
    threads[itm.threadID] = thread;
    signals[itm.objID] = signal;
}

void W3POBase::wait(const Item& itm)
{
    auto thread = threads[itm.threadID];
    checkThreadInit(thread.id, thread.tVc);
    auto signal = signals[itm.objID];
    thread.tVc.sync(signal);
    thread.tVc.add(thread.id, 1);
    threads[itm.threadID] = thread;
}

void W3POBase::atomicOp(const Item& itm)
{
    auto thread = threads[itm.threadID];
    auto atomic = atomics[itm.objID];

    thread.tVc.sync(atomic);
}

void W3POWrd::read(const Item& itm)
{
    W3POBase::read(itm);

    auto thread = threads[itm.threadID];
    auto var = variables[itm.objID];

    thread.tVc.sync(var.lastWrite);
    auto& lastFE = var.frontier.back();
    lastFE.vc = thread.tVc;

    variables[itm.objID] = var;
    threads[itm.threadID] = thread;
}

void W3POWrd::write(const Item& itm)
{
    W3POBase::write(itm);

    auto thread = threads[itm.threadID];
    auto var = variables[itm.threadID];
    var.lastWrite = thread.tVc;
    var.lastWrite.add(itm.threadID, -1);
    variables[itm.objID] = var;
}
