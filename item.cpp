#include "item.h"
#include "penalty.h"
#include "settings.h"
#include "box.h"


Item::Item(ItemType type) : type_(type), p_(0)
{
	prev_ = nullptr;
	next_ = nullptr;
}


Item::~Item()
{
}

Item* Item::after()
{
	if (next_ == nullptr)
	{
		return this;
	}

	if (next_->type() == BOX)
	{
		return next_;
	}
	else if (next_->type() == PENALITY)
	{
		if (next_->p() < -1000)
			return next_;
	}
	return next_->after();
}

void Item::SVG(ofstream& file) const
{
	file << "<g transform='translate(" << x_ << ", " << y_ << ")'>";

	//all kinds of borders
	if (settings::border_[type_])
	{
		file << "<path fill='none' class='border" << type_ << "' d='M 0 0 L 0 " << height_ << " " << width_ << " " << height_ << " " << width_ << " 0 0 0'/>";
	}

	if (type_ == BOX)
	{
		Glyph* glyph = static_cast<const Box*>(this)->glyph();
		//a char
		file << "<g transform='translate(" << -glyph->hori_bearing_x() << ", " << -glyph->hori_bearing_y() << ")'>\n";

		file << "<use xlink:href='#char" << int(glyph->content()) << "' fill='rgb(0,0,0)'";
		file << "/>";

		file << "</g>";
	}
	else
	{
		//reverse for line
		if (type_ == LINE)
		{
			file << "<g transform = 'scale(1, -1) translate(0, " << -settings::descender_ - height_ << ")'>\n";
		}
		//a recurse
		if (type_ == PAGE || type_ == LINE)
		{
			const vector<Item*>* children = static_cast<const Container*>(this)->children();
			for (Item* child : *children)
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