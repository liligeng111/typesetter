#pragma once

#include "path.h"
#include "algebra.h"

class Box
{
public:
	enum BoxType
	{
		CHAR = 0,
		WORD = 1,
		LINE = 2
	};

	Box();
	~Box();

private:
	Box* parent_;
	Box** children_;
	Path* path_;
	Matrix matrix_;
};

