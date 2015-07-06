#pragma once

#include <string>
#include <ftoutln.h>
#include "point.h"

using namespace std;

class Path
{
public:
	Path(FT_Outline outline);
	~Path();

	string SVG() const { return svg_; }
	const char* c_str() const const { return svg_.c_str(); }

private:
	int n_contours_;
	int n_points_;
	Point* points_;
	char* tags_;
	int* contours_;
	int flags_;
	string svg_;
};

