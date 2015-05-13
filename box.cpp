#include "box.h"
#include "typesetter.h"

Box::Box(Glyph* glyph, Word* word) : Item(Item::BOX)
{
	glyph_ = glyph;
	word_ = word;
	x_ = 0;
	y_ = 0;
	width_ = 0;

	if (word_ != nullptr)
	{
		word_->add_child(this);
	}
}

Box::~Box()
{
}

