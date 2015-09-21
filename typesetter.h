#pragma once

#include <string>
#include <ft2build.h>
#include <map> 
#include <vector>
#include <list>
#include <map>
#include FT_FREETYPE_H
#include "river.h"
#include "breakpoint.h"
#include "typesetter.h"
#include "container.h"
#include <qstring>
#include "lib\libhyphenate\Hyphenator.h"

using namespace std;

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
	
	void render(RenderTarget target);
	void render_page(RenderTarget target, int page);
	Page* page(int i) { return pages_[i]; }
	int page_count() const { return pages_.size(); }
	static void message(const string& msg);
	void typeset(QString& text, bool single_paragraph);

	void LoadFace();

	Page* get_page_number(int qscintilla_line_number);
	int get_prev_magic(int line);
	int get_next_magic(int line);

	vector<pair<int, float>> get_suggestions() { return suggestions_; }

	void reset_magic_index() { magic_index_ = 0; }
	QString magic_index_increase();
	QString magic_index_decrease();

	
private:

	FT_Library  library_;
	FT_Face face_;
	map<wchar_t, Glyph*> glyph_cache_;
	long hyphen_width_;
	Glyph* hyphen_glyph_;

	Hyphenate::Hyphenator hyphenator_;

	//input sequence
	list<Item*> items_;
	list<Item*> paragraph_;

	vector<Line*> lines_;
	vector<Page*> pages_;
	list<Breakpoint*> breakpoints;
	list<Breakpoint*> active_list_;
	list<Breakpoint*> passive_list_;
	vector<vector<River*>> rivers_;
	vector<int> magic_paragraphs_;
		
	void clean();

	void break_paragraph();

	void optimum_fit_magic_edge(vector<pair<Item*, Item*>> magic_found);
	void optimum_fit();
	void reverse_optimum_fit();
	void bidirection_magic();
	void optimum_fit_with_magic();
	void fill_lines();

	void justify();

	void detect_river();


	void A_star();
	map<pair<Item*, Item*>, Breakpoint::Demerits> local_cost_;


	unsigned long sum_stretchability_[settings::item_priority_size_];
	unsigned long sum_shrinkability_[settings::item_priority_size_];
	void reset_changeability() 
	{
		std::fill(sum_shrinkability_, sum_shrinkability_ + settings::item_priority_size_, 0);
		std::fill(sum_stretchability_, sum_stretchability_ + settings::item_priority_size_, 0);
	};

	void set_changeability(int priority, unsigned long stretchability, unsigned long shrinkability)
	{
		sum_stretchability_[0] += stretchability;
		sum_shrinkability_[0] += shrinkability;
		sum_stretchability_[priority] += stretchability;
		sum_shrinkability_[priority] += shrinkability;
	};

	//magic stuff
	//priority_queue<Item*> magic_queue_;

	//TODO::useless
	int qscintilla_line; //corresponding line num in qsccintilla
	int magic_line; //previously found magic line

	vector<pair<int, float>> suggestions_;
	int paragraph_number_;

	//if typesetting a single paragraph or the whole passage
	bool typesetting_single_paragraph_;
	vector<pair<Item*, Item*>> magic_edges_;
	int magic_index_; //index of the megic edge currently using
	float normal_demerits_;

};
