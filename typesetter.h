#pragma once

#include <string>
#include <ft2build.h>
#include <vector>
#include FT_FREETYPE_H
#include "box.h"
#include "river.h"

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

	void LoadFace();

private:

	string content_;
	FT_Library  library_;
	FT_Face face_;
	Box* root_;
	vector<Box*> words_;
	vector<Box*> lines_;
	vector<River*> rivers_;
	long line_height_;
	
	void Clean();

	void RaggedRight();
	void FirstFit();
	void BestFit();
	void OptimumFit();
	void Justify();
	void DetectRiver();
};
