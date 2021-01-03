#include "tsanv1.h"
    
ThreadSanitizer::~ThreadSanitizer()
{}

void ThreadSanitizer::read(const Item& itm)
{

}

void ThreadSanitizer::write(const Item& itm)
{

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

}

void ThreadSanitizer::wait(const Item& itm)
{

}

void ThreadSanitizer::atomicOp(const Item& itm)
{

}
