#include "breakpoint.h"


Breakpoint::Breakpoint(Item* item)
{
	init(item, 0, 0, Demerits(0, 0, 0, 0, 0), nullptr);
}

Breakpoint::Breakpoint(Item* item, int line, float demerits_sum, Demerits demerits, Breakpoint* prev)
{
	init(item, line, demerits_sum, demerits, prev);
}

void Breakpoint::init(Item* item, int line, float demerits_sum, Demerits demerits, Breakpoint* prev)
{
	item_ = item;
	line_ = line;
	demerits_sum_ = demerits_sum;
	demerits_ = demerits;
	prev_ = prev;
}

Breakpoint::~Breakpoint()
{
	//TODO::memory leak

}
