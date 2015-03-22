#pragma once

#include "path.h"
#include "glyph.h"

class Box
{
public:
	enum BoxType
	{
		CHAR = 0,
		WORD = 1,
		LINE = 2
	};

	Box(Glyph* glyph);
	~Box();

	Glyph* glyph() { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }

	long x() const { return x_; }
	long y() const { return y_; }
	void set_x(long x) { x_ = x; }
	void set_y(long y) { y_ = y; }

	string SVG(int n) const;
	
private:
	Box* parent_;
	Box** children_;
	Path* path_;
	long x_;
	long y_;
	Glyph* glyph_;
	//Matrix matrix_;
};

