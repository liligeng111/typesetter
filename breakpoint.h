#pragma once

#include "box.h"

class Breakpoint
{
public:
	Breakpoint(Box* box) : box_(box) {};
	~Breakpoint();

	long EndAt() const { return box_->EndAt(); }
	long x() const { return box_->x(); }
	Box* box() const { return box_; }

private:
	Box* box_;
	Breakpoint* prev_;
};

