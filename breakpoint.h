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


	void set_length(long l) { length_ = l; }
	long length() const { return length_; }

private:
	Box* box_;
	Breakpoint* prev_;
	long length_;
};

