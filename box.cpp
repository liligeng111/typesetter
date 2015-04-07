#include "box.h"
#include "typesetter.h"
#include "settings.h"

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
	Clear();
}

void Box::ExpandBox(Box* box)
{
	long width = box->x_ + box->width_;
	long height = box->y_ + box->height_;
	width_ = width > width_ ? width : width_;
	height_ = height > height_ ? height : height_;
}

void Box::Clear()
{
	// kill all the children!
	for (Box* child : children_)
	{
		delete child;
	}
	children_ = vector<Box*>();
}

string Box::SVG() const
{
	string str = "";
	str += "<g transform='translate(" + to_string(x_) + ", " + to_string(y_) + ")'>";

	//all kinds of borders
	if (settings::border_[type_])
	{
		str += "<path fill='none' stroke-width='32' stroke='rgb" + settings::border_color[type_] + "' d='M 0 0 L 0 " + to_string(height_) + " " + to_string(width_) + " " + to_string(height_) + " " + to_string(width_) + " 0 0 0'/>";
	}

	if (glyph_ != NULL)
	{
		//a char
		str += "<g transform='translate(" + to_string(-glyph_->hori_bearing_x())  + ", " + to_string(-glyph_->hori_bearing_y()) + ")'>\n";
		str += "<path fill='rgb(0, 0, 0)' d='";
		str += glyph_->path()->SVG();
		str += "'/>";
		str += "</g>";
	}
	else
	{	
		//reverse for line
		if (type_ == LINE)
		{
			str += "<g transform = 'scale(1, -1) translate(0, " + to_string(-descender_ - height_) + ")'>\n";
		}
		//a recurse
		for (Box* child : children_)
		{
			str += child->SVG();
		}
		if (type_ == LINE)
		{
			str += "</g>";
		}
	}

	str += "</g>";
	return str;
}
