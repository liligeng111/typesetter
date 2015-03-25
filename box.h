#pragma once

#include "path.h"
#include "glyph.h"
#include <vector>

class Box
{
public:
	enum BoxType
	{
		SPACE = 0,
		CHAR = 1,
		WORD = 2,
		LINE = 3,
		PAGE = 4
	};

	//Box() : Box(NULL, NULL) {};
	Box(Box* parent, BoxType type) : Box(NULL, parent, type) {};
	Box(Glyph* glyph, Box* parent, BoxType type);
	~Box();

	Glyph* glyph() { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }

	const Box* parent() const { return parent_; }
	const vector<Box*>* children() const { return &children_; }
	void AddChild(Box* child) { children_.push_back(child); }
	void set_parent(Box* parent) { parent_ = parent; parent_->AddChild(this); }
	void Clear();

	long x() const { return x_; }
	long y() const { return y_; }
	long width() const { return width_; }
	long height() const { return height_; }
	//void set_x(long x) { x_ = x; }
	//void set_y(long y) { y_ = y; }
	//void set_width(long width) { width_ = width; }
	//void set_height(long height) { height_ = height; }
	void set_geometry(long x, long y, long width, long height) { x_ = x; y_ = y; width_ = width; height_ = height; }
	void Translate(long x, long y) { x_ += x; y_ += y; }
	void ExpandBox(Box* box);

	string SVG() const;
	
private:
	BoxType type_;
	Box* parent_;
	vector<Box*> children_;
	long x_;
	long y_;
	long width_;
	long height_;
	Glyph* glyph_;
	//Matrix matrix_;
};

