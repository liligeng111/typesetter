#pragma once

struct Point
{
	Point()
	{
		x = 0;
		y = 0;
	};
	Point(unsigned long xx, unsigned long yy)
	{
		x = xx;
		y = yy;
	};
	unsigned long x;
	unsigned long y;
};
