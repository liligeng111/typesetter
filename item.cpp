#include <iomanip>
#include "item.h"
#include "settings.h"
#include "container.h"


Item::Item(ItemType type) : type_(type)
{
	//geometry
	width_ = 0;
	height_ = 0;
	x_ = 0;
	y_ = 0;

	is_magic_ = false;

	//penalty
	is_break_ = false;
	penalty_ = 0;
	breakable_ = false;
	break_width_ = 0;
	break_glyph_ = nullptr;

	//shrink and stretch
	stretchability_ = 0;
	shrinkability_ = 0;
	std::fill(sum_shrinkability_, sum_shrinkability_ + settings::item_priority_size_, 0);
	std::fill(sum_stretchability_, sum_stretchability_ + settings::item_priority_size_, 0);
	stretch_ratio_ = 0;
	shrink_ratio_ = 0;
	expansion_ = 1; // for font expansion
	priority_ = settings::item_priority_size_ - 1; // just in case

	glyph_ = nullptr;
	word_ = nullptr;

	parent_ = nullptr;
	prev_ = nullptr;
	next_ = nullptr;
}

void Item::init_box(Glyph* glyph, Word* word, long y, long z)
{
	glyph_ = glyph;
	word_ = word;

	if (word_ != nullptr)
	{
		word_->add_child(this);
	}
	priority_ = settings::box_priority_;
	stretchability_ = y;
	shrinkability_ = z;
	stretch_ratio_ = settings::font_expansion_;
	shrink_ratio_ = settings::font_expansion_;
}

void Item::init_glue(long y, long z, int priority)
{
	stretchability_ = y;
	shrinkability_ = z;
	priority_ = priority;
	stretch_ratio_ = settings::stretch_ratio_;
	shrink_ratio_ = settings::shrink_ratio_;
	breakable_ = true;
}

void Item::init_letter_space(long y, long z)
{
	stretchability_ = y;
	shrinkability_ = z;
	priority_ = settings::letter_space_priority_;
	stretch_ratio_ = settings::letter_space_stretch_ratio_;
	shrink_ratio_ = settings::letter_space_shrink_ratio_;
}

void Item::init_penalty(int p)
{
	penalty_ = p;
	priority_ = settings::box_priority_;
	breakable_ = true;
}

Item::~Item()
{
}

void Item::set_changeability(unsigned long* sum_stretchability, unsigned long* sum_shrinkability)
{
	for (int i = 0; i < settings::item_priority_size_; i++)
	{
		sum_stretchability_[i] = sum_stretchability[i];
		sum_shrinkability_[i] = sum_shrinkability[i];
	}
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
		if (next_->penalty() <= -1000)
			return next_;
	}
	return next_->after();
}

Item* Item::before()
{
	if (prev_ == nullptr)
	{
		return this;
	}

	if (prev_->type() == BOX)
	{
		return prev_;
	}
	else if (prev_->type() == PENALITY)
	{
		if (prev_->penalty() <= -1000)
			return prev_;
	}
	return prev_->before();
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
		file << "<g transform='scale(" << expansion_ << ", 1)'>\n";
		file << "<path fill='none' class='border" << type_ << "' d='M 0 0 L 0 " << height_ << " " << width_ << " " << height_ << " " << width_ << " 0 0 0'/>";
		file << "</g>\n";
	}

	if (glyph_ != nullptr)
	{
		//a char
		file << "<g transform='scale(" << expansion_ << ", 1)'>\n";
		file << "<g transform='translate(" << -glyph_->hori_bearing_x() << ", " << -glyph_->hori_bearing_y() << ")'>\n";
		file << "<use xlink:href='#char" << int(glyph_->content()) << "' fill='rgb(0,0,0)'/>\n";
		file << "</g>\n";
		file << "</g>\n";
	}
	else if (is_break_ && break_glyph_ != nullptr)
	{
		file << "<g transform='translate(" << width_ - break_glyph_->hori_bearing_x() << ", " << 0 << ")'>\n";
		file << "<use xlink:href='#char" << int(break_glyph_->content()) << "' fill='rgb(0,0,0)'/>\n";
		file << "</g>\n";
	}
	else
	{
		//reverse for line
		if (type_ == LINE)
		{
			const Line* line = static_cast<const Line*>(this);
			file << "<text class='markdown markdown_total_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().r << "</text>";
			file << "<text class='line_number' x='" << -settings::em_size_ << "' y='" << settings::line_height_ / 1.5 << "' fill='blue' font-size='" << settings::em_size_ / 3 << "px'>" << line->line_number() << "</text>";
			file << "<text style='display:none;' class='markdown markdown_penalty' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().penalty << "</text>";
			file << "<text style='display:none;' class='markdown markdown_demerits' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->demerits().result << "</text>";
			file << "<text style='display:none;' class='markdown markdown_space_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->space_r() << "</text>";
			file << "<text style='display:none;' class='markdown markdown_font_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->font_r() << "</text>";
			file << "<text style='display:none;' class='markdown markdown_letter_space_r' x='" << settings::content_width_point() + settings::em_size_ / 2 << "' y='" << settings::line_height_ << "' fill='red' font-size='" << settings::em_size_ / 3 << "px'>" << line->letter_space_r() << "</text>";
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


string Item::content() 
{
	if (word_ != nullptr)
	{
		return *word_->content();
	}
	return "item_type: " + to_string(type_); 
}