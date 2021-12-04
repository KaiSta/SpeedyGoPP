#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "util.h"
#include "register.h"
#include "racedetector.h"

class W3POBase : public RaceDetector
{
public:
    W3POBase() = default;
    W3POBase(const W3POBase&) = delete;
    void operator=(const W3POBase&) = delete;
    virtual ~W3POBase();

    virtual void read(const Item&) override;
	virtual void write(const Item&) override;
	virtual void lock(const Item&) override;
	virtual void unlock(const Item&) override;
	virtual void signal(const Item&) override;
	virtual void wait(const Item&) override;
	virtual void atomicOp(const Item&) override;

protected:
    typedef std::pair<thread_t, int> Epoch;

    struct Thread {
        thread_t id;
        VectorClock tVc;
        Lockset tLockset;
    };
    std::unordered_map<thread_t, Thread> threads;

    struct Lock {
        VectorClock rel;
        std::vector<VectorClock> rels;
        VectorClock hb;
        Epoch acq;
        Epoch nextAcq;
        size_t count;
        bool strongSync;
        bool fullSync;
    };
    std::unordered_map<obj_t, Lock> locks;

    struct Dot {
        size_t id;
        VectorClock vc;
        int sourceRef;
        thread_t tid;
        bool write;
        Lockset ls;
        std::unordered_map<obj_t, int> lsCount;
    };
    struct Variable {
        std::vector<Dot> frontier;
        VectorClock lastWrite;
        Lockset lwLocks;
        size_t current;
    };
    std::unordered_map<obj_t, Variable> variables;

    std::unordered_map<obj_t, VectorClock> signals;

    std::unordered_map<obj_t, VectorClock> atomics;

    void checkThreadInit(thread_t, VectorClock&);
};

class W3POWrd : public W3POBase
{
public:
    W3POWrd() = default;
    W3POWrd(const W3POWrd&) = delete;
    void operator=(const W3POWrd&) = delete;
    virtual ~W3POWrd();

    virtual void read(const Item&) override;
	virtual void write(const Item&) override;
};

class W3PO : public W3POBase
{
public:
    W3PO() = default;
    W3PO(const W3PO&) = delete;
    void operator=(const W3PO&) = delete;
    virtual ~W3PO();

    virtual void read(const Item&) override;
	virtual void write(const Item&) override;
	virtual void lock(const Item&) override;
	virtual void unlock(const Item&) override;
};
