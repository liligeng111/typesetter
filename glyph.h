#pragma once

#include <ftglyph.h>
#include "path.h"

class Glyph
{
public:
	Glyph(FT_GlyphSlot glyph);
	~Glyph();

	Path* path() { return &path_; }
	long linear_hori_advance() const { return linear_hori_advance_; }
	long linear_vert_advance() const { return linear_vert_advance_; }
	Vector3l advance() const { return advance_; }

	
private:
	Path path_;
	long linear_hori_advance_;
	long linear_vert_advance_;
	Vector3l advance_;
};

