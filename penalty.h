#pragma once

#include "item.h"

class Penalty: public Item
{
public:
	Penalty(int p);
	~Penalty();
	string content() { return "PENALTY " + to_string(p_); }

};

