#include "box.h"
#include "typesetter.h"
#include "settings.h"
#include <fstream>
#include <iostream>

long Box::descender_ = 0;

Box::Box(Glyph* glyph, Box* parent, BoxType type)
{
	glyph_ = glyph;
	parent_ = parent;
	left_ = NULL;
	right_ = NULL;
	type_ = type;
	x_ = 0;
	y_ = 0;
	width_ = 0;
	children_ = vector<Box*>();
	justify_ = true;

	if (parent != NULL)
	{
		parent->AddChild(this);
	}

	content_ = "";
	MYWORD = NULL;
}

Box::~Box()
{
	// kill all the children!
	for (Box* child : children_)
	{
		delete child;
	}
}

void Box::ExpandBox(Box* box)
{
	long width = box->x_ + box->width_ - x_;
	long height = box->y_ + box->height_ - y_;
	width_ = width > width_ ? width : width_;
	height_ = height > height_ ? height : height_;
}


void Box::SVG(ofstream& file) const
{
	file << "<g transform='translate(" << x_ << ", " << y_ << ")'>";

	//all kinds of borders
	if (settings::border_[type_])
	{
		file << "<path fill='none' class='border" << type_ << "' d='M 0 0 L 0 " << height_ << " " << width_ << " " << height_ << " " << width_ << " 0 0 0'/>";
	}

	if (glyph_ != NULL)
	{
		//a char
		file << "<g transform='translate(" << -glyph_->hori_bearing_x() << ", " << -glyph_->hori_bearing_y() << ")'>\n";

		file << "<use xlink:href='#char" << int(glyph_->content()) << "' fill='rgb(0,0,0)' class='char char" << int(glyph_->content()) << "'";
		file << "/>";

		file << "</g>";
	}
	else
	{	
		//reverse for line
		if (type_ == LINE)
		{
			file << "<g transform = 'scale(1, -1) translate(0, " << -descender_ - height_ << ")'>\n";
		}
		//a recurse
		if (type_ != BACKSPACE)
		{
			for (Box* child : children_)
			{
				child->SVG(file);
			}
		}
		if (type_ == LINE)
		{
			file << "</g>";
		}
	}

	file << "</g>";
}

Box* Box::NearestChild(long l) const
{
	long dis = 2147483647;
	int n = -1;
	int count = 0;

	for (std::string::size_type i = 0; i < hyphenated_.size(); i++)
	{
		if (hyphenated_[i] != '-')
			continue;
		count++;
		long d = abs(children_[i - count]->EndAt() - l);
		if (dis > d)
		{
			dis = d;
			n = i - count;
		}
	}
	if (n == -1)
		return NULL;
	return children_[n];
}