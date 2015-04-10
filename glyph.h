#pragma once

#include <ftglyph.h>
#include "path.h"

class Glyph
{
public:
	Glyph(FT_GlyphSlot glyph, char ch);
	~Glyph();

	Path* path() { return &path_; }
	long linear_hori_advance() const { return linear_hori_advance_; }
	long linear_vert_advance() const { return linear_vert_advance_; }
	Vector3l advance() const { return advance_; }
	long width() const { return width_; }
	long height() const { return height_; }
	long hori_bearing_x() const { return hori_bearing_x_; }
	long hori_bearing_y() const { return hori_bearing_y_; }
	char content() const { return content_; }

	
private:
	Path path_;
	long linear_hori_advance_;
	long linear_vert_advance_;
	Vector3l advance_;
	long width_;
	long height_;
	char content_;

	long hori_bearing_x_;
	long hori_bearing_y_;
};

