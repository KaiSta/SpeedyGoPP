#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "util.h"
#include "register.h"

#include "racedetector.h"

class FastTrack : public RaceDetector
{
public:
	FastTrack() = default;
	FastTrack(const FastTrack&) = delete;
	void operator=(const FastTrack&) = delete;
	virtual ~FastTrack();

	void checkThreadInit(thread_t, VectorClock&);

	virtual void read(const Item&) override;
	virtual void write(const Item&) override;
	virtual void lock(const Item&) override;
	virtual void unlock(const Item&) override;
	virtual void signal(const Item&) override;
	virtual void wait(const Item&) override;
	virtual void atomicOp(const Item&) override;

protected:
	std::unordered_map<thread_t, VectorClock> threads;
	std::unordered_map<obj_t, VectorClock> locks;
	std::unordered_map<obj_t, std::pair<VectorClock, VectorClock> > vars;//1 read 2 write
	std::unordered_map<obj_t, VectorClock> signals;
	std::unordered_map<obj_t, VectorClock> volatiles;
};

// namespace FastTrack
// {
// 	class initializer {
// 	public:
// 		initializer()
// 		{
// 			auto& r = Register::getReg();
// 			std::vector<std::function<void(const Item&)> > funcs{std::bind(&fastTrack::put, &ft, std::placeholders::_1)};
// 			r.add("fastTrack", funcs, std::bind(&fastTrack::setOutput, &ft, std::placeholders::_1));
// 		}
// 		initializer(const initializer&) = delete;
// 		initializer(initializer&&) = delete;
// 		void operator=(const initializer&) = delete;
// 		void operator=(initializer&&) = delete;
// 	private:
// 		fastTrack ft;
// 	};

// 	static const initializer ftInit;
// }