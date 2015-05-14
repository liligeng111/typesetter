#pragma once

#include "item.h"

class Glue: public Item
{
public:
	Glue(long w, long y, long z);
	~Glue();

	string content() { return "__"; }

private:
};

