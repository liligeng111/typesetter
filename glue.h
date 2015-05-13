#pragma once

#include "item.h"

class Glue: public Item
{
public:
	Glue(long w, long y, long z);
	~Glue();

	long stretchability() { return stretchability_; }
	long shrinkability() { return shrinkability_; }

private:
	long stretchability_;
	long shrinkability_; 
};

