#pragma once

#include <string>
#include <ftoutln.h>
#include "algebra.h"

using namespace std;

class Path
{
public:
	Path(FT_Outline outline);
	~Path();

	string SVG() const { return svg_; }

private:
	int n_contours_;
	int n_points_;
	Vector3l* points_;
	char* tags_;
	int* contours_;
	int flags_;
	string svg_;
};

