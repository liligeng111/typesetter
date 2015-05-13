#pragma once

#include "item.h"
#include <vector>

class Box;
class Container : public Item
{
public:
	Container(Item::ItemType type);
	~Container();
	void add_child(Item* item) { children_.push_back(item); item->set_parent(this); }

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
};

class Line : public Container
{
public:
	Line();
	~Line();
	void set_r(float r) { r_ = r; }
	float r() { return r_; }

private:
	float r_;
};

class Page : public Container
{
public:
	Page();
	~Page();
};