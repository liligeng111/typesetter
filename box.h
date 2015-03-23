#pragma once

#include "path.h"
#include "glyph.h"
#include <vector>

class Box
{
public:/*
	enum BoxType
	{
	CHAR = 0,
	WORD = 1,
	LINE = 2
	};*/

	Box() : Box(NULL, NULL) {};
	Box(Box* parent) : Box(NULL, parent) {};
	Box(Glyph* glyph, Box* parent);
	~Box();

	Glyph* glyph() { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }

	const Box* parent() const { return parent_; }
	const vector<Box*>* children() const { return &children_; }
	void AddChild(Box* child) { children_.push_back(child); }

	long x() const { return x_; }
	long y() const { return y_; }
	long width() const { return width_; }
	void set_x(long x) { x_ = x; }
	void set_y(long y) { y_ = y; }
	void set_width(long width) { width_ = width; }
	void Transform(long x, long y) { x_ += x; y_ += y; }

	string SVG() const;
	
private:
	//BoxType type;
	Box* parent_;
	vector<Box*> children_;
	long x_;
	long y_;
	long width_;
	Glyph* glyph_;
	//Matrix matrix_;
};

