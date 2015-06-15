#include "container.h"

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

void Word::hyphenate(Hyphenate::Hyphenator* hyphenator, Glyph* hyphen)
{
	hyphenated_ = hyphenator->hyphenate(*content_);
	const char *cstr = hyphenated_.c_str();
	//cout << hyphenated_ << endl;

	int j = 0;
	for (int i = 0; i < children_.size(); i++)
	{
		if (children_[i]->glyph()->content() == cstr[j])
		{
			//allow break after user input hyphen, ignore last one
			if (i != children_.size() - 1 && cstr[j] == '-')
			{
				Item* space = children_[i]->next();
				if (space->type() != Item::LETTER_SPACE)
				{
					cout << "Error hyphenating" << *content_ << " " << hyphenated_ << endl;
				}

				space->set_breakable(true);
			}
		}
		else if (cstr[j] != '-')
		{
			cout << "Error hyphenating " << *content_ << " " << hyphenated_ << " " << int(children_[i]->glyph()->content()) << " " << int(cstr[j]) << ' ' << i << " " << j << endl;
		}
		else
		{
			// hyphen found
			j++;
			Item* temp = children_[i];
			Item* space = temp->prev();
			space->set_breakable(true);
			space->set_break_width(hyphen->width() + space->width());
			space->set_break_glyph(hyphen);
			space->set_penalty(50);
		}
		j++;
	}
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