#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "glyph.h"
#include "settings.h"

using namespace std;

class Container;
class Word;

class Item
{

public:

	enum ItemType
	{
		PENALITY = -1,
		GLUE = 0,
		BOX = 1,

		WORD = 2,
		LINE = 3,
		PAGE = 4, 

		LETTER_SPACE = 5
	};

	Item(ItemType type);
	void init_box(Glyph* glyph, Word* word, long y, long z);
	void init_glue(long y, long z, int priority);
	void init_letter_space(long y, long z);
	void init_penalty(int p);
	~Item();

	ItemType type() { return type_; }
	long x() const { return x_; }
	long y() const { return y_; }
	void set_x(long x) { x_ = x; }
	void set_y(long y) { y_ = y; }
	long width() const { return width_; }
	long height() const { return height_; }
	void set_width(long width) { width_ = width; }
	void set_height(long height) { height_ = height; }
	void set_geometry(long x, long y, long width, long height) { x_ = x; y_ = y; width_ = width; height_ = height; }
	void translate(long x, long y) { x_ += x; y_ += y; }
	Point mid_point() const { return Point(x_ + width_ / 2, y_ + height_ / 2); }
	unsigned long end_at() const { return width_ + x_; }
	unsigned long stretchability() { return stretchability_; }
	unsigned long shrinkability() { return shrinkability_; }
	unsigned long sum_stretchability(int priority) { return sum_stretchability_[priority]; }
	unsigned long sum_shrinkability(int priority) { return sum_shrinkability_[priority]; }
	Glyph* glyph() const { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }

	void set_changeability(unsigned long* sum_stretchability, unsigned long* sum_shrinkability_);
	void set_expansion(float expansion) { expansion_ = expansion; }
	float expansion() { return expansion_; }
	float stretch_ratio() const { return stretch_ratio_; }
	float shrink_ratio() const { return shrink_ratio_; }
	int priority() const {return priority_; }
	void set_priority(int priority) { priority_ = priority;	}

	bool is_break() const { return is_break_; }
	void set_is_break(bool is_break) { is_break_ = is_break; }
	bool breakable() const { return breakable_; }
	void set_breakable(bool breakable) { breakable_ = breakable; }
	long break_width() const { return break_width_; }
	void set_break_width(long break_width) { break_width_ = break_width; }
	Glyph* break_glyph() const { return break_glyph_; }
	void set_break_glyph(Glyph* break_glyph) { break_glyph_ = break_glyph; }
	int penalty() const { return penalty_; }
	void set_penalty(int penalty) { penalty_ = penalty; }

	void set_parent(Container* parent) { parent_ = parent; }
	void set_prev(Item* prev);
	Item* prev() const { return prev_; }
	Item* next() const { return next_; }
	Item* after();  //used by optimum fit
	Item* before();  
	
	void SVG(ofstream& file) const;

	//for debug
	string content();
	virtual string word_content() { return content(); }


	bool is_magic() const { return is_magic_; }
	void set_is_magic(bool is_magic) { is_magic_ = is_magic; }

	//TODO::temp
	int shrink_count_;
	int stretch_count_;
protected:
	const ItemType type_;

	//geometry
	long width_, height_;
	long x_, y_;

	//penalty
	int penalty_;
	bool is_break_; // if it is a breakpoint.
	bool breakable_; // if it is allowed to break after this item.
	long break_width_; // width if break after item. (width of hyphen + letter_space)
	Glyph* break_glyph_; // item inserterd if break after item. (hyphen)

	//shrink and stretch
	int priority_;
	unsigned long stretchability_;
	unsigned long shrinkability_;
	unsigned long sum_stretchability_[settings::item_priority_size_];
	unsigned long sum_shrinkability_[settings::item_priority_size_]; //index 0 is reserved for sum of sum
	float stretch_ratio_; // for font expansion
	float shrink_ratio_;
	float expansion_;

	Glyph* glyph_; //for box and penalty
	Word* word_; // for box

	Container* parent_;
	Item* prev_;
	Item* next_;


	bool is_magic_;
};

