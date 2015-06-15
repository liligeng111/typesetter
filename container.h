#pragma once

#include "item.h"
#include "breakpoint.h"
#include <list>
#include <vector>
#include "lib\libhyphenate\Hyphenator.h"

class Container : public Item
{
public:

	Container(Item::ItemType type);
	~Container();
	virtual void add_child(Item* item) { children_.push_back(item); item->set_parent(this); }

	Item* child(int i) const { return children_[i]; }
	const vector<Item*>* children() const { return (&children_); }
	Item* first() const { return children_[0]; }
	Item* last() const { return children_[children_.size() - 1]; }

	void expand_box(Item* box);

protected:
	vector<Item*> children_; 
};

class Word: public Container
{
public:
	Word();
	~Word();
	string* content() const { return content_; }
	string word_content() { return *content_; }
	string hyphenated() const { return hyphenated_; }
	void add_child(Item* item) { children_.push_back(item); }

	void hyphenate(Hyphenate::Hyphenator* hyphenator, Glyph* hyphen);

private:
	string* content_;
	string hyphenated_;
};

class Line : public Container
{
public:
	Line();
	~Line();
	void set_demerits(Breakpoint::Demerits demerits) { demerits_ = demerits; }
	Breakpoint::Demerits demerits() const { return demerits_; }
	void set_r(float font, float space, float letter_space) { font_r_ = font; space_r_ = space; letter_space_r_ = letter_space; }
	float space_r() const { return space_r_; }
	float font_r() const { return font_r_; }
	float letter_space_r() const { return letter_space_r_; }
	long line_number() const { return line_number_; }
	void set_line_number(long number) { line_number_ = number; }

private:
	Breakpoint::Demerits demerits_;
	float font_r_, space_r_, letter_space_r_;
	long line_number_;
};

class Page : public Container
{
public:
	Page();
	~Page();
};