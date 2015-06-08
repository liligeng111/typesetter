#pragma once

#include <string>
#include <ft2build.h>
#include <map> 
#include <vector>
#include <list>
#include <map>
#include <chrono>
#include FT_FREETYPE_H
#include "box.h"
#include "glue.h"
#include "penalty.h"
#include "river.h"
#include "breakpoint.h"
#include "typesetter.h"

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

	void render(RenderTarget target);
	void render_page(RenderTarget target, int page);
	int page_count() const { return pages_.size(); }
	static void message(const string& msg);
	void Typeset();

	void LoadFace();

private:

	string file_;
	FT_Library  library_;
	FT_Face face_;
	map<char, Glyph*> glyph_cache_;
	long hyphen_width_;
	Glyph* hyphen_glyph_;

	//input sequence
	list<Item*> items_;
	list<Item*> paragraph_;

	vector<Box*> words_;
	vector<Line*> lines_;
	vector<Page*> pages_;
	list<Breakpoint*> breakpoints;
	list<Breakpoint*> active_list_;
	list<Breakpoint*> passive_list_;
	vector<vector<River*>> rivers_;
	chrono::high_resolution_clock::time_point start_time_;

	void Progress(string msg);
	
	void clean();

	void break_paragraph();

	void optimum_fit();
	void reverse_optimum_fit();
	void insert_hyphen(string hyphenated);
	void fill_lines();

	void justify();

	void detect_river();


	void A_star();
	map<pair<Item*, Item*>, Breakpoint::Demerits> local_cost_;
};
