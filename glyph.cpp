#include "glyph.h"
#include <iostream>

Glyph::Glyph(FT_GlyphSlot glyph, wchar_t ch) : path_(glyph->outline)
{
	linear_hori_advance_ = glyph->linearHoriAdvance;
	linear_vert_advance_ = glyph->linearVertAdvance;
	advance_ = Point(glyph->metrics.horiAdvance, glyph->metrics.vertAdvance);
	width_ = glyph->metrics.width;
	height_ = glyph->metrics.height;
	hori_bearing_x_ = glyph->metrics.horiBearingX;
	hori_bearing_y_ = glyph->metrics.horiBearingY;
	content_ = ch;

	//margin kerning
	left_protruding_ = 0;
	right_protruding_ = 0;

	//TODO:: Better way for this && quotation mark
	if (ch == '(')
	{
		left_protruding_ = 50;
	}
	else if (ch == 'A')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'J')
	{
		left_protruding_ = 50;
	}
	else if (ch == 'T')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'V')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'W')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'X')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'Y')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'v')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'w')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'x')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == 'y')
	{
		left_protruding_ = 50;
		right_protruding_ = 50;
	}
	else if (ch == '-')
	{
		right_protruding_ = 700;
	}
	else if (ch == ',')
	{
		right_protruding_ = 700;
	}
	else if (ch == ';')
	{
		right_protruding_ = 500;
	}
	else if (ch == ':')
	{
		right_protruding_ = 500;
	}
	else if (ch == 150)
	{
		//en-dash
		right_protruding_ = 300;
	}
	else if (ch == 151)
	{
		//em-dash
		right_protruding_ = 200;
	}
	else if (ch == '!')
	{
		right_protruding_ = 200;
	}
	else if (ch == '?')
	{
		right_protruding_ = 200;
	}
	else if (ch == ')')
	{
		right_protruding_ = 50;
	}
	else if (ch == 'F')
	{
		right_protruding_ = 50;
	}
	else if (ch == 'K')
	{
		right_protruding_ = 50;
	}
	else if (ch == 'L')
	{
		right_protruding_ = 50;
	}
	else if (ch == 'k')
	{
		right_protruding_ = 50;
	}
	else if (ch == 'r')
	{
		right_protruding_ = 50;
	}
	else if (ch == 't')
	{
		right_protruding_ = 50;
	}
}


Glyph::~Glyph()
{
}
