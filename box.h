#pragma once

#include "item.h"
#include "container.h"

class Box: public Item
{
public:
	Box(Glyph* glyph, Word* word);
	~Box();
	
	Word* word() const { return word_; }
	void set_word(Word* word) { word_ = word; }
	string content() { return glyph_ == nullptr ? "EMPTY BOX" : string(1, glyph_->content()); }
		
private:
	Word* word_;
};

