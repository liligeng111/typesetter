#include "container.h"
#include "box.h"

Container::Container(Item::ItemType type) : Item(type)
{
}


Container::~Container()
{
	for (Item* item : children_)
	{
		delete item;
	}
}

void Container::expand_box(Item* item)
{
	long width = item->x() + item->width() - x_;
	long height = item->y() + item->height() - y_;
	width_ = width > width_ ? width : width_;
	height_ = height > height_ ? height : height_;
}

Word::Word() : Container(Item::WORD)
{
	content_ = new string;
}


Word::~Word()
{
	delete content_;
}

void Word::hyphenate(Hyphenate::Hyphenator* hyphenator)
{
	hyphenated_ = hyphenator->hyphenate(*content_);
}

Line::Line() : Container(Item::LINE)
{
}


Line::~Line()
{
}

Page::Page() : Container(Item::PAGE)
{
}


Page::~Page()
{
}