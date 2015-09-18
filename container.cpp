#include "container.h"
#include "logger.h"

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
	if (content_ == nullptr)
	{
		cout << "Error hyphenating: Empty word" << endl;
		return;
	}

	hyphenated_ = hyphenator->hyphenate(*content_);
	const char *cstr = hyphenated_.c_str();
	//cout << hyphenated_ << endl;

	int j = 0;
	for (int i = 0; i < children_.size(); i++)
	{
		if (content_->at(i) == cstr[j])
		{
			//allow break after user input hyphen, ignore last one
			if (i != 0 && i != children_.size() - 1 && cstr[j] == '-')
			{
				Item* space = children_[i]->next();
				if (space->type() != Item::LETTER_SPACE)
				{
					string msg = "Error hyphenating" + *content_ + " " + hyphenated_;
					Logger::error(100, msg);
				}

				space->set_breakable(true);
			}
		}
		else if (cstr[j] != '-')
		{
			string msg = "Error hyphenating " + *content_ + " " + hyphenated_ + " " + to_string(int(children_[i]->glyph()->content())) + " " + to_string(int(cstr[j])) + ' ' + to_string(i) + " " + to_string(j);
			Logger::error(101, msg);
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
	font_r_ = 0;
	space_r_ = 0;
	letter_space_r_ = 0;
	line_number_ = 0;
	qscintilla_line_number_ = 0;
	page_ = nullptr;
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