#include "glyph.h"


Glyph::Glyph(FT_GlyphSlot glyph, char ch) : path_(glyph->outline)
{
	linear_hori_advance_ = glyph->linearHoriAdvance;
	linear_vert_advance_ = glyph->linearVertAdvance;
	advance_ = Vector3l(glyph->metrics.horiAdvance, glyph->metrics.vertAdvance);
	width_ = glyph->metrics.width;
	height_ = glyph->metrics.height;
	hori_bearing_x_ = glyph->metrics.horiBearingX;
	hori_bearing_y_ = glyph->metrics.horiBearingY;
	content_ = ch;
}


Glyph::~Glyph()
{
}
