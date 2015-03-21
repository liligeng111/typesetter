#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "viewer.h"

using namespace std;

class Typesetter
{

public:
	enum RenderTarget
	{
		SVG = 0
	};

	Typesetter();
	~Typesetter();
	void Render(RenderTarget target);


private:
	static void Message(const string& msg) { Viewer::Message(msg); }

	string content_;
	FT_Library  library_;
	FT_Face face_;
};
