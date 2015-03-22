#pragma once

#include <string>
#include <ft2build.h>
#include <vector>
#include FT_FREETYPE_H
#include "box.h"

using namespace std;

class Box;
class Glyph;

class Viewer;
class Typesetter
{

public:
	enum RenderTarget
	{
		SVG = 0
	};

	Typesetter();
	~Typesetter();

	void set_content(string& content) { content_ = content; }

	void Render(RenderTarget target);
	static void Message(const string& msg);
	void Typeset();


private:

	string content_;
	FT_Library  library_;
	FT_Face face_;
	vector<Box> boxes_;

	void clean() { boxes_ = vector<Box>(); };

};
