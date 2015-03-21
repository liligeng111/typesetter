#include "viewport.h"
#include <math.h>


Viewport::Viewport(float x_min, float y_min, float x_max, float y_max)
{
	x_min_ = floorf(x_min + 0.5f);
	y_min_ = floorf(y_min + 0.5f);
	x_max_ = floorf(x_max + 0.5f);
	y_max_ = floorf(y_max + 0.5f);
}


Viewport::~Viewport()
{
}
