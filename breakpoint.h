#pragma once

#include "item.h"

class Breakpoint
{
public:
	Breakpoint(Item* item);
	Breakpoint(Item* item, int line, float demerits, float r, Breakpoint* prev);
	~Breakpoint();

	Item* item() const { return item_; }
	int line() const { return line_; }
	float demerits() const { return demerits_; }
	float r() const { return r_; }
	Breakpoint* prev() const { return prev_; }
	Breakpoint* next() const { return next_; }

private:
	Item* item_;
	int line_; //# of line end at here
	float demerits_;
	float r_;
	Breakpoint* prev_;
	Breakpoint* next_;
};

