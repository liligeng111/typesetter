#pragma once

#include <string>
#include <ft2build.h>
#include <map> 
#include <vector>
#include <list>
#include <chrono>
#include FT_FREETYPE_H
#include "box.h"
#include "glue.h"
#include "penalty.h"
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
	long hyphen_width_;

	//input sequence
	vector<Item*> items_;

	vector<Box*> words_;
	vector<Line*> lines_;
	vector<Page*> pages_;
	list<Breakpoint*> active_list_;
	list<Breakpoint*> passive_list_;
	vector<vector<River*>> rivers_;
	chrono::high_resolution_clock::time_point start_time_;
	Breakpoint* start_; //first breakpoint

	void Progress(string msg);
	
	void clean();

	void optimum_fit();
	void break_lines();
	void justify();

	void detect_river();
};
