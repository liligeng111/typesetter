#include "box.h"
#include "typesetter.h"
#include "settings.h"
#include <fstream>

long Box::descender_ = 0;

Box::Box(Glyph* glyph, Box* parent, BoxType type)
{
	glyph_ = glyph;
	parent_ = parent;
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
	long width = box->x_ + box->width_;
	long height = box->y_ + box->height_;
	width_ = width > width_ ? width : width_;
	height_ = height > height_ ? height : height_;
}


void Box::SVG(ofstream& file, bool cache) const
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
		file << "<path class='char char" << int(glyph_->content()) << "'";
		if (!cache)
			file << "d='" << glyph_->path()->SVG() << "'";
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
		for (Box* child : children_)
		{
			child->SVG(file, cache);
		}
		if (type_ == LINE)
		{
			file << "</g>";
		}
	}

	file << "</g>";
}
