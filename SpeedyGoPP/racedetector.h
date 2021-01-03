#pragma once

#include <memory>

class RaceDetector {
public:

typedef std::function<void(int,int)> reportFunc;

RaceDetector() = default;
RaceDetector(const RaceDetector&) = delete;
void operator=(const RaceDetector&) = delete;

virtual ~RaceDetector(){};

void setReporter(reportFunc rep) {
    report = rep;
}

virtual void read(const Item&) = 0;
virtual void write(const Item&) = 0;
virtual void lock(const Item&) = 0;
virtual void unlock(const Item&) = 0;
virtual void signal(const Item&) = 0;
virtual void wait(const Item&) = 0;
virtual void atomicOp(const Item&) = 0;

protected:
reportFunc report;
};