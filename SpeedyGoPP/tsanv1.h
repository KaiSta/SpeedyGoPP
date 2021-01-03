#pragma once

#include "util.h"
#include "racedetector.h"

class ThreadSanitizer : public RaceDetector {
public:
    ThreadSanitizer() = default;
    ThreadSanitizer(const ThreadSanitizer&) = delete;
    void operator=(const ThreadSanitizer&) = delete;
    virtual ~ThreadSanitizer();

	virtual void read(const Item&) override;
	virtual void write(const Item&) override;
	virtual void lock(const Item&) override;
	virtual void unlock(const Item&) override;
	virtual void signal(const Item&) override;
	virtual void wait(const Item&) override;
	virtual void atomicOp(const Item&) override;

protected:
	struct access {
		VectorClock vc;
		Lockset ls;
		Item itm;
	};
						//read				//write
	typedef std::pair<std::vector<access>, std::vector<access> > accessHistory;
	
    std::unordered_map<thread_t, std::pair<VectorClock, Lockset> > threads;
    std::unordered_map<obj_t, VectorClock> signals;
	std::unordered_map<obj_t, VectorClock> volatiles;
	std::unordered_map<obj_t, accessHistory> vars; // first=read,second=write
};