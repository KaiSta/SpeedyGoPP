#pragma once

#include "fastTrack.h"

class SHB : public FastTrack 
{
public:
    SHB() = default;
    SHB(const SHB&) = delete;
    void operator=(const SHB&) = delete;
    virtual ~SHB();

    virtual void read(const Item&) override;
    virtual void write(const Item&) override;

protected:
    std::unordered_map<obj_t, VectorClock> lastWrite;
};