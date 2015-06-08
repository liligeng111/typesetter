#pragma once

#include "item.h"
#include "breakpoint.h"
#include <list>
#include <vector>
#include "lib\libhyphenate\Hyphenator.h"

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
	string* content() const { return content_; }
	string word_content() { return *content_; }
	string hyphenated() const { return hyphenated_; }

	void hyphenate(Hyphenate::Hyphenator* hyphenator);

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

private:
	Breakpoint::Demerits demerits_;
};

class Page : public Container
{
public:
	Page();
	~Page();
};