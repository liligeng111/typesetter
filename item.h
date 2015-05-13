#pragma once

#include <algebra.h>
#include <fstream>
#include <iostream>

using namespace std;

class Container;
class Item
{

public:
	~Item();

	enum ItemType
	{
		PENALITY = -1,
		GLUE = 0,
		BOX = 1,
		WORD = 2,
		LINE = 3,
		PAGE = 4
	};

	ItemType type() { return type_; }
	long x() const { return x_; }
	long y() const { return y_; }
	int p() const { return p_; }
	void set_x(long x) { x_ = x; }
	void set_y(long y) { y_ = y; }
	long width() const { return width_; }
	long height() const { return height_; }
	void set_width(long width) { width_ = width; }
	void set_height(long height) { height_ = height; }
	void set_geometry(long x, long y, long width, long height) { x_ = x; y_ = y; width_ = width; height_ = height; }
	void translate(long x, long y) { x_ += x; y_ += y; }
	Vector3l mid_point() const { return Vector3l(x_ + width_ / 2, y_ + height_ / 2); }
	long end_at() const { return width_ + x_; }

	void set_parent(Container* parent) { parent_ = parent; }
	void set_prev(Item* prev) { prev_ = prev; if (prev_ != nullptr) prev->next_ = this; }
	Item* prev() const { return prev_; }
	Item* next() const { return next_; }
	Item* after();  //used by optimum fit

	//used by optimum fit algorithm
	unsigned long long sum_w_, sum_y_, sum_z_;

	void SVG(ofstream& file) const;

protected:
	Item(ItemType type);
	const ItemType type_;

	long width_;
	long height_;
	long x_;
	long y_;
	int p_; //penalty

	Container* parent_;
	Item* prev_;
	Item* next_;
};

