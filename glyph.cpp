#include "glyph.h"


Glyph::Glyph(FT_GlyphSlot glyph) : path_(glyph->outline)
{
	linear_hori_advance_ = glyph->linearHoriAdvance;
	linear_vert_advance_ = glyph->linearVertAdvance;
	advance_ = Vector3l(glyph->advance.x, glyph->advance.y);
}


Glyph::~Glyph()
{
}
