#include "box.h"
#include "typesetter.h"

Box::Box(Glyph* glyph)
{
	glyph_ = glyph;
	x_ = 0;
	y_ = 0;
}

Box::~Box()
{
}

string Box::SVG(int n) const
{
	string str = "";
	str += "<g transform = \"translate(" + to_string(x_) + ", " + to_string(y_) + ")\">";
	str += "<path id=\"p" + to_string(n) + "\" fill=\"rgb(0, 0, 0)\" d=\"";
	str += glyph_->path()->SVG();
	str += "\"/></g>";
	return str;
}