#include "SHB.h"

SHB::~SHB() {}

void SHB::read(const Item& itm)
{
   	auto thread = threads[itm.threadID];
	checkThreadInit(itm.threadID, thread);

	auto var = vars[itm.objID];

	if (!var.second.less(thread)) //compare with write history
	{
		report(itm.sourceRef, SrcRefManager::NotAvailable);
	}

    auto lw = lastWrite[itm.objID];
    thread.sync(lw);

	var.first.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}

void SHB::write(const Item& itm)
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

    lastWrite[itm.objID] = thread;

	var.second.sync(thread);
	thread.add(itm.threadID, 1);

	threads[itm.threadID] = thread;
	vars[itm.objID] = var;
}