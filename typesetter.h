#pragma once

#include <string>
#include <ft2build.h>
#include <map> 
#include <vector>
#include <chrono>
#include FT_FREETYPE_H
#include "box.h"
#include "river.h"
#include "breakpoint.h"

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

	void set_content(string& content) { file_ = content; }

	void Render(RenderTarget target);
	static void Message(const string& msg);
	void Typeset();

	void LoadFace();

private:

	string file_;
	FT_Library  library_;
	FT_Face face_;
	map<char, Glyph*> glyph_cache_;
	vector<Box*> words_;
	vector<Box*> lines_;
	vector<Box*> pages_;
	vector<Breakpoint*> breakpoints_;
	vector<vector<River*>> rivers_;
	chrono::high_resolution_clock::time_point start_time_;

	void Progress(string msg);
	
	void Clean();

	void RaggedRight();
	void FirstFit();
	void BestFit();
	void OptimumFit();

	void BreakLines();
	void Justify();

	void DetectRiver();
};
