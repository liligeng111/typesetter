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
	Path* path = glyph_->path();
	if (!(path->points_))
	{
		Typesetter::Message("Uninitialized glyph");
	}

	string str = "<path id=\"p" + to_string(n) + "\" fill=\"rgb(0, 0, 0)\" d=\"";

	int j = 0;
	for (int i = 0; i < path->n_contours_; i++)
	{
		bool first = true;
		str += " M ";
		for (; j <= path->contours_[i]; j++)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				if (path->tags_[i] & (1 << 0))
					str += " L ";
				else
					str += " Q ";
			}
			str += to_string(path->points_[j].x() + x_) + " " + to_string(path->points_[j].y() + y_);
		}
		str += " Z";
	}

	str += "\"/>";
	return str;
}