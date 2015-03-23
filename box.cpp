#include "box.h"
#include "typesetter.h"

Box::Box(Glyph* glyph, Box* parent)
{
	glyph_ = glyph;
	parent_ = parent;
	x_ = 0;
	y_ = 0;
	width_ = 0;
	children_ = vector<Box*>();

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

string Box::SVG() const
{
	string str = "";
	str += "<g transform = \"translate(" + to_string(x_) + ", " + to_string(y_) + ")\">";

	if (glyph_ != NULL)
	{
		//a char
		str += "<path fill=\"rgb(0, 0, 0)\" d=\"";
		str += glyph_->path()->SVG();
		str += "\"/>";
	}
	else
	{
		//a word
		for (Box* child : children_)
		{
			str += child->SVG();
		}
	}

	str += "</g>";
	return str;
}