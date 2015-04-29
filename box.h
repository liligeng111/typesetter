#pragma once

#include "path.h"
#include "glyph.h"
#include <vector>
#include "lib\libhyphenate\Hyphenator.h"

class Box
{
public:
	enum BoxType
	{
		BACKSPACE = -1,
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

	Glyph* glyph() const { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }	
	BoxType type() const { return type_; }

	const Box* parent() const { return parent_; }
	const Box* left() const { return left_; }
	const Box* right() const { return right_; }
	const vector<Box*>* children() const { return &children_; }
	const string& content() const{ return content_; }
	Box* child(int i) const { return children_[i]; }
	Box* First() const { return children_[0]; }
	Box* Last() const { return children_[children_.size() - 1]; }
	Box* NearestChild(long l) const;
	void AddChild(Box* child) { children_.push_back(child); if (type_ == WORD) content_ += child->glyph_->content(); }
	int ChildrenSize() const { return children_.size(); }
	void set_parent(Box* parent) { parent_ = parent; parent_->AddChild(this); }
	void set_left(Box* left) { left_ = left; if (left != NULL) left->right_ = this; }

	long x() const { return x_; }
	long y() const { return y_; }
	long width() const { return width_; }
	long height() const { return height_; }
	long EndAt() const { return width_ + x_; }
	bool justify() const { return justify_; }
	void set_x(long x) { x_ = x; }
	void set_y(long y) { y_ = y; }
	void set_width(long width) { width_ = width; }
	void set_height(long height) { height_ = height; }
	static void set_descender(long descender) { descender_ = descender; }
	void set_geometry(long x, long y, long width, long height) { x_ = x; y_ = y; width_ = width; height_ = height; }
	void set_justify(bool justify) { justify_ = justify; }
	void Translate(long x, long y) { x_ += x; y_ += y; }
	void ExpandBox(Box* box);
	Vector3l MidPoint() const { return Vector3l(x_ + width_ / 2, y_ + height_ / 2); }

	void SVG(ofstream& file, bool cache) const;
	void Hyphenate(Hyphenate::Hyphenator* hyphenator) { hyphenated_ = hyphenator->hyphenate(content_); }
	
private:
	BoxType type_;
	Box* parent_;
	Box* left_;
	Box* right_;
	vector<Box*> children_;
	long x_;
	long y_;
	long width_;
	long height_;
	static long descender_;
	Glyph* glyph_;
	bool justify_;
	//Matrix matrix_;
	string content_, hyphenated_;
};

