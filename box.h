#pragma once

#include "path.h"
#include "glyph.h"
#include "item.h"
#include "container.h"

class Box: public Item
{
public:
	Box(Glyph* glyph, Word* word);
	~Box();

	Word* word() const { return word_; }

	Glyph* glyph() const { return glyph_; }
	void set_glyph(Glyph* glyph) { glyph_ = glyph; }	
		
private:
	Glyph* glyph_;
	Word* word_;
};

