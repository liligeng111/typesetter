#include "container.h"
#include "box.h"

Container::Container(Item::ItemType type) : Item(type)
{
}


Container::~Container()
{
	//TODO::delete
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
}


Word::~Word()
{
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