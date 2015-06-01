#include "breakpoint.h"


Breakpoint::Breakpoint(Item* item)
{
	init(item, 0, 0, 0, nullptr);
}

Breakpoint::Breakpoint(Item* item, int line, float demerits, float r, Breakpoint* prev)
{
	init(item, line, demerits, r, prev);
}

void Breakpoint::init(Item* item, int line, float demerits, float r, Breakpoint* prev)
{
	item_ = item;
	line_ = line;
	demerits_ = demerits;
	r_ = r;
	prev_ = prev;
}

Breakpoint::~Breakpoint()
{
	//TODO::memory leak

}
