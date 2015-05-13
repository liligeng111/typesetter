#include "glue.h"


Glue::Glue(long w, long y, long z) : Item(Item::GLUE)
{
	width_ = w;
	stretchability_ = y;
	shrinkability_ = z;
}


Glue::~Glue()
{
}
