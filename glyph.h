#pragma once

#include <ftglyph.h>
#include "path.h"

class Glyph
{
public:
	Glyph(FT_GlyphSlot glyph, wchar_t ch);
	~Glyph();

	Path* path() { return &path_; }
	long linear_hori_advance() const { return linear_hori_advance_; }
	long linear_vert_advance() const { return linear_vert_advance_; }
	Point advance() const { return advance_; }
	long width() const { return width_; }
	long height() const { return height_; }
	long hori_bearing_x() const { return hori_bearing_x_; }
	long hori_bearing_y() const { return hori_bearing_y_; }
	int left_protruding() const { return left_protruding_; }
	int right_protruding() const { return right_protruding_; }
	wchar_t content() const { return content_; }

	
private:
	Path path_;
	long linear_hori_advance_;
	long linear_vert_advance_;
	Point advance_;
	long width_;
	long height_;
	wchar_t content_;

	long hori_bearing_x_;
	long hori_bearing_y_;

	//margin kerning
	int left_protruding_;
	int right_protruding_;
};

