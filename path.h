#pragma once

#include <string>
#include <ftoutln.h>
#include "algebra.h"

using namespace std;

class Path
{
public:
	friend class Box;
	Path(FT_Outline outline);
	~Path();

private:
	int n_contours_;
	int n_points_;
	Vector3l* points_;
	char* tags_;
	int* contours_;
	int flags_;
};

