#include <iomanip>
#include "item.h"
#include "penalty.h"
#include "settings.h"
#include "box.h"


Item::Item(ItemType type) : type_(type), p_(0), stretchability_(0), shrinkability_(0), glyph_(nullptr)
{
	prev_ = nullptr;
	next_ = nullptr;
	expansion_ = 1;
}


Item::~Item()
{
}

void Item::set_prev(Item* prev)
{
	prev_ = prev; 
	if (prev_ == nullptr)
		return;
	if (prev->next_ != nullptr)
	{
		//insertion
		next_ = prev->next_;
		prev->next_->prev_ = this;
	}
	prev->next_ = this;
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
		if (next_->p() <= -1000)
			return next_;
	}
	return next_->after();
}

void Item::SVG(ofstream& file) const
{
	if (type_ == LINE)
		file << "<g class='line' transform='translate(" << x_ << ", " << y_ << ")'>";
	else
		file << "<g transform='translate(" << x_ << ", " << y_ << ")'>";

	//all kinds of borders
	if (settings::border_[type_])
	{
		file << "<path fill='none' class='border" << type_ << "' d='M 0 0 L 0 " << height_ << " " << width_ << " " << height_ << " " << width_ << " 0 0 0'/>";
	}

	if (glyph_ != nullptr)
	{
		//a char
		file << "<g transform='translate(" << 0 << ", " << -glyph_->hori_bearing_y() << ")'>\n";
		file << "<use g transform = 'scale(" << expansion_ << ", 1)' xlink:href='#char" << int(glyph_->content()) << "' fill='rgb(0,0,0)'/>";
		file << "</g>";
	}
	else
	{
		//reverse for line
		if (type_ == LINE)
		{
			const Line* line = static_cast<const Line*>(this);
			file << "<text class='markdown markdown_total_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().r << "</text>";
			file << "<text style='display:none;' class='markdown markdown_penalty' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().penalty << "</text>";
			file << "<text style='display:none;' class='markdown markdown_demerits' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().result << "</text>";
			file << "<text style='display:none;' class='markdown markdown_space_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->space_r() << "</text>";
			file << "<text style='display:none;' class='markdown markdown_font_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->font_r() << "</text>";
			file << "<g transform = 'scale(1, -1) translate(0, " << -settings::descender_ - height_ << ")'>\n";
		}
		//a ->
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