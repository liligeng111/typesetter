#include "breakpoint.h"


Breakpoint::Breakpoint(Item* item) : Breakpoint(item, 0, 0, 0, nullptr)
{

}

Breakpoint::Breakpoint(Item* item, int line, float demerits, float r, Breakpoint* prev)
{
	item_ = item;
	line_ = line;
	demerits_ = demerits;
	r_ = r;
	prev_ = prev;
}


Breakpoint::~Breakpoint()
{
}
