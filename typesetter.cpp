#include "typesetter.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "viewer.h"
#include "settings.h"
#include "lib\libhyphenate\Hyphenator.h"

using namespace Hyphenate;

Typesetter::Typesetter()
{
	//just to be sure
	file_ = "";
	LoadFace();
}

void Typesetter::LoadFace()
{
	// init all th FT stuff
	FT_Error error = FT_Init_FreeType(&library_);
	if (error)
	{
		Message("An error occurred during FT library initialization.");
	}

	error = FT_New_Face(library_, "./fonts/MinionPro-Regular.otf", 0, &face_);
	if (error == FT_Err_Unknown_File_Format)
	{
		Message("This font format is unsupported.");
	}
	else if (error)
	{
		Message("Unable to read the font file or it is broken.");
	}

	error = FT_Set_Char_Size(face_, 0, settings::font_size_ * 64, 0, 72);
	if (error)
	{
		Message("Cannot set char size");
	}
	// according to http://www.freetype.org/freetype2/docs/glyphs/glyphs-2.html
	//error = FT_Set_Pixel_Sizes(face_, 0, settings::font_size_ * settings::dpi_ / 72);
	//if (error)
	//{
	//	Message("Cannot set pixel size");
	//}

	settings::descender_ = face_->descender;
	settings::em_size_ = face_->units_per_EM;
	settings::line_height_ = 1.2 * face_->units_per_EM;


	error = FT_Load_Char(face_, '-', FT_LOAD_NO_SCALE);
	if (error)
	{
		Message("Error loading character");
	}

	settings::space_width_ = face_->glyph->advance.x;
	settings::stretchability_ = settings::space_width_ / 3;
	settings::shrinkability_ = settings::space_width_ / 3;
}

Typesetter::~Typesetter()
{
}

void Typesetter::Message(const string& msg)
{ 
	Viewer::Message(msg); 
}

void Typesetter::clean()
{
	for (Page* page : pages_)
	{
		delete page;
	}

	for (auto rivers : rivers_)
	{
		for (auto river : rivers)
		{
			delete river;
		}
	}

	items_.clear();
	paragraph_.clear();
	words_.clear();
	lines_.clear();
	pages_.clear();
	rivers_.clear();
	breakpoints.clear();
	active_list_.clear();
	passive_list_.clear();
}

void Typesetter::Progress(string msg)
{
	cout << fixed << setprecision(3) << chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time_).count() / 1000.0 << "s--" << msg << "\n";
}

void Typesetter::insert_hyphen(string hyphenated)
{
	//cout << hyphenated << endl;
	auto last = paragraph_.rbegin();
	for (int i = hyphenated.length() - 1; i >= 0; i--)
	{
		if (hyphenated[i] != '-')
		{
			last++;
			continue;
		}
		//TODO::is that the correct penalty?
		if ((*last)->glyph() == hyphen_glyph_)
		{
			Penalty* penalty = new Penalty(0);
			penalty->set_geometry((*last)->end_at(), 0, 0, 0);
			penalty->sum_y_ = (*last)->sum_y_;
			penalty->sum_z_ = (*last)->sum_z_;
			penalty->set_prev((*last));
			penalty->set_glyph(nullptr);
			paragraph_.insert(last.base(), penalty);
			last++;
			last++;
		}
		else
		{
			Penalty* penalty = new Penalty(50);
			penalty->set_geometry((*last)->end_at(), hyphen_glyph_->hori_bearing_y(), hyphen_width_, hyphen_glyph_->height());
			penalty->sum_y_ = (*last.base())->sum_y_;
			penalty->sum_z_ = (*last.base())->sum_z_;
			penalty->set_prev((*last));
			penalty->set_glyph(hyphen_glyph_);
			paragraph_.insert(last.base(), penalty);
			last++;
		}
	}
}

void Typesetter::Typeset()
{
	start_time_ = chrono::high_resolution_clock::now();

	Hyphenator hyphenator = (RFC_3066::Language("en"));

	FT_Error error = FT_Load_Char(face_, '-', FT_LOAD_NO_SCALE);
	if (error)
	{
		Message("Error loading character");
	}

	//load hyphen glyph
	hyphen_glyph_ = new Glyph(face_->glyph, '-');
	glyph_cache_['-'] = hyphen_glyph_;
	hyphen_width_ = hyphen_glyph_->advance().x();
	Glyph* glyph;

	Progress("Cleaning previous data");
	//clear previous data
	clean();

	Progress("Typesetting");
	cout << "Page width: " << settings::content_width_point() << endl;

	unsigned long long x_cursor = 0;
	unsigned long long sum_y = 0;
	unsigned long long sum_z = 0;

	unsigned char last_ch = 0;
	Item* last_item = nullptr;
	Word* word = nullptr;
	unsigned char ch = 0;
	fstream fin(file_, fstream::in);
	bool new_paragraph = true;
	unsigned char next_ch;

	while (true)
	{
		fin >> noskipws >> next_ch;
		ch = next_ch;
		cout << int(ch) << endl;
		//what it is?
		if (ch < 0)
		{
			continue;
		}

		//new line margin
		if (new_paragraph)
		{
			new_paragraph = false;
			Box* box = new Box(nullptr, NULL);
			box->set_geometry(x_cursor, 0, 0, 0);
			x_cursor += box->width();
			paragraph_.push_back(box);
			box->sum_y_ = sum_y;
			box->sum_z_ = sum_z;
			box->set_prev(last_item);
			last_item = box;
		}

		//backspace
		if (ch == '\n' || fin.eof())
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator);
				insert_hyphen(word->hyphenated());
			}
			word = nullptr;
			//must break
			Glue* glue = new Glue(0, 100000000, 0);
			glue->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(glue);
			glue->sum_y_ = sum_y;
			glue->sum_z_ = sum_z;
			sum_y += glue->stretchability();
			sum_z += glue->shrinkability();
			glue->set_prev(last_item);
			Penalty* penalty = new Penalty(-1000);
			penalty->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(penalty);
			penalty->sum_y_ = sum_y;
			penalty->sum_z_ = sum_z;
			penalty->set_prev(glue);
			last_ch = ch;
			last_item = penalty;
			if (optimum_fit() != 0)
				return;
			sum_y = 0; 
			sum_z = 0;
			new_paragraph = true;
			if (fin.eof())
				break;
			continue;
		}
		//new space
		else if (ch == ' ')
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator);
				insert_hyphen(word->hyphenated());
			}
			word = nullptr;
			Glue* glue = new Glue(settings::space_width_, settings::stretchability_, settings::shrinkability_);
			glue->set_geometry(x_cursor, 0, settings::space_width_, 0);
			paragraph_.push_back(glue);
			glue->sum_y_ = sum_y;
			glue->sum_z_ = sum_z;
			sum_y += glue->stretchability();
			sum_z += glue->shrinkability();
			glue->set_prev(last_item);
			last_item = glue;
		}
		//new char
		else
		{
			if (word == nullptr)
			{
				word = new Word;
			}
			word->content()->append(1, ch);

			auto cache_index = glyph_cache_.find(ch);
			//seach cache
			if (cache_index == glyph_cache_.end())
			{
				FT_Error error = FT_Load_Char(face_, ch, FT_LOAD_NO_SCALE);
				if (error)
				{
					Message("Error loading character");
				}

				glyph = new Glyph(face_->glyph, ch);
				glyph_cache_[ch] = glyph;
			}
			else
			{
				glyph = cache_index->second;
			}

			Box* box = new Box(glyph, NULL);
			box->set_geometry(x_cursor, box->glyph()->hori_bearing_y(), glyph->advance().x(), -box->glyph()->height());
			paragraph_.push_back(box);
			box->sum_y_ = sum_y;
			box->sum_z_ = sum_z;
			box->set_prev(last_item);
			last_item = box;
		}
		last_ch = ch;


		//kern
		FT_Vector* kern = new FT_Vector();
		//? what are the modes?
		FT_Get_Kerning(face_, last_ch, ch, FT_KERNING_DEFAULT, kern);
		x_cursor += kern->x;

		if (ch == ' ')
		{
			x_cursor += settings::space_width_;
		}
		else
		{
			x_cursor += glyph->advance().x();
		}

		//current_box->ExpandBox(box);
	}


	//cout << "cache size: " << glyph_cache_.size() << endl;
	Progress("Breaking paragraphs");
	break_lines();
	Progress("Justify lines");
	justify();

	if (settings::show_river_)
	{
		Progress("Detecting River");
		detect_river();
	}
	Progress("Typesetting Done");
}

void Typesetter::detect_river()
{
	/*
	for (int p = 0; p < pages_.size(); p++)
	{
		Box* page = pages_[p];
		rivers_.push_back(vector<River*>());
		const vector<Box*>* lines = page->children();


		//wtf is this bug
		int max_i = lines->size() - 2;
		for (int i = 0; i < max_i; i++)
		{
			for (int b = 0; b < lines->at(i)->ChildrenSize(); b++)
			{
				Box* box = lines->at(i)->child(b);
				if (box->type() != Box::BoxType::SPACE || box == box->parent()->child(box->parent()->ChildrenSize() - 1) || box->width() >  2 * settings::space_width_)
					continue;	
				
				//check up for duplicate
				Box* up = NULL;
				for (int t = 0; i > 0 && t < lines->at(i - 1)->ChildrenSize(); t++)
				{
					Box* temp = lines->at(i - 1)->child(t);
					if (temp->type() != Box::BoxType::SPACE || temp == temp->parent()->child(temp->parent()->ChildrenSize() - 1) || temp->width() >  2 * settings::space_width_)
						continue;
					//if (labs(box->MidPoint().x() - temp->MidPoint().x()) < settings::river_threshold_ * (box->width() + temp->width()) / 2)
					if (labs(box->MidPoint().x() - temp->MidPoint().x()) < settings::river_threshold_ * settings::space_width_)
					{
						up = temp;
						break;
					}
				}

				if (up != NULL)
					continue;
				
				River* river = new River(p);
				Box* last = box;
				bool found = true;
				int search = i;
				while (found)
				{
					river->AddBox(last);
					found = false;	
					search++;

					for (int t = 0; search < lines->size() && t < lines->at(search)->ChildrenSize(); t++)
					{
						Box* temp = lines->at(search)->child(t);
						if (temp->type() != Box::BoxType::SPACE || temp == temp->parent()->child(temp->parent()->ChildrenSize() - 1) || temp->width() >  2 * settings::space_width_)
							continue;
						if (labs(last->MidPoint().x() - temp->MidPoint().x()) < settings::river_threshold_ * settings::space_width_)
						{
							last = temp;
							found = true;
							break;
						}
					}
				}

				if (river->size() >= 2)
				{
					rivers_[p].push_back(river);
				}
				else
				{
					delete river;
				}
			}
		}
	}
	*/
}

int Typesetter::optimum_fit()
{
	//line width 
	active_list_.clear();
	long l = settings::content_width_point();
	Box* box = new Box(nullptr, nullptr);
	box->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	paragraph_.front()->set_prev(box);
	paragraph_.push_front(box);
	active_list_.push_back(new Breakpoint(box));
	auto iter = paragraph_.begin();
	iter++;

	while (iter != paragraph_.end())
	{
		Item* b = *iter;
		//cout << b << " : " << b->content() << endl;
		iter++;
		//if (b->x() != b->sum_w_)
		//	cout << b->content() << "  " << b->x() << " " << b->sum_w_ << endl;
		Breakpoint* breakpoint = nullptr;
		int pb = b->p(); //penalty
		if (b->type() == Item::BOX)
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->p() > 999)
			{
				continue;
			}
		}

		//potential break
		auto a = active_list_.begin();
		auto end = active_list_.end();

		//iterare the active list
		while (a != end)
		{
			//calculate r
			bool forced_break = false;
			Item* after = (*a)->item()->after();
			float r;
			long L = b->x() - after->x();
			if (b->type() == Item::PENALITY)
			{
				L += b->width();
				forced_break = pb < -999;
			}
			if (L < l)
			{
				long Y = b->sum_y_ - after->sum_y_;
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_z_ - after->sum_z_;
				if (Z > 0)
					r = 1.0f * (l - L) / Z;
				else
					r = 1000;
			}
			else
			{
				//prefect!
				r = 0;
			}

			//if (L == 0)
			//{
			//	for (Item* item : paragraph_)
			//	{
			//		//cout << item << " : " << item->content() << endl;
			//	}
			//	cout << after->content() << " ----> " << b->content() << " L: " << L << " r: " << r << " a: " << after->sum_y_ << " b: " << b->sum_y_ << endl;
			//	cout << (*a)->item()->content() << " ----> " << b->content() << endl << endl;
			//}
			//cout << "r " << r << endl;
			//rho here
			if (r >= -1 && r < 3)
			{
				//cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
				float d;
				if (pb >= 0)
				{
					d = 1 + 100 * r * r * abs(r) + pb;
					d = d * d;
				}
				else if (pb > -1000)
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d - pb * pb;
				}
				else
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d;
				}
				//TODO::f(a) and c
				//new breakpoint
				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits() + d, r, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
				//better breakpoint
				else if ((*a)->demerits() + d < breakpoint->demerits())
				{
					delete breakpoint;
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits() + d, r, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
			}

			if (r < -1 || forced_break)
			{
				//cout << "deletefrom active list: " << *a << endl;
				passive_list_.push_back(*a);
				a = active_list_.erase(a);
			}
			else
			{
				a++;
			}
		}
		//TODO::what is q?
		if (breakpoint != nullptr)
		{
			active_list_.push_back(breakpoint);
		}

	}
	if (active_list_.empty())
	{
		Message("ERROR: Unable to preform optimum fit");
		return 1;
	}

	//reverse list
	Breakpoint* bp = active_list_.front();
	auto pos = breakpoints.rbegin();
	while (bp->prev() != nullptr)
	{
		//cout << bp->item()->after()->content() << endl;
		breakpoints.insert(pos.base(), bp);
		bp = bp->prev();
		pos++;
	}
	//merge paragraph
	items_.splice(items_.end(), paragraph_);

	return 0;
}

void Typesetter::break_lines()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Page* current_page = new Page();
	current_page->set_geometry(settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_), settings::content_width_point(), settings::content_height_point());
	pages_.push_back(current_page);
	Line* current_line = new Line();
	current_line->set_geometry(0, y_adjust, 0, settings::line_height_);
	current_page->add_child(current_line);
	lines_.push_back(current_line);

	Item* last = nullptr;
	auto iter = breakpoints.begin();
	
	//check each item
	for (Item* child : items_)
	{
		//cout << child->content() << " " << child->x() << " " << child->end_at() << " " << x_adjust << endl;

		child->translate(x_adjust, 0);
		if (child->type() == Item::BOX || child->type() == Item::GLUE)
			current_line->add_child(child);
		if (iter != breakpoints.end() && child == (*iter)->item())
		{
			//need newline
			//new page
			//cout << "Breaking at: " << (*iter) << " " << (*iter)->item()->after()->content() << endl;
			//cout << "L: " << (*iter)->L << " a: " << (*iter)->a << " b: " << (*iter)->b << endl << endl;
			if (y_adjust + settings::line_height_ > settings::content_height_point())
			{
				y_adjust = 0;
				current_page = new Page();
				current_page->set_geometry(settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_), settings::content_width_point(), settings::content_height_point());
				pages_.push_back(current_page);
			}

			if (child->type() == Item::PENALITY)
			{
				current_line->add_child(child);
			}
			x_adjust = -child->after()->x();

			if (current_line != nullptr)
			{
				current_line->set_r((*iter)->r());
				//cout << breakpoint->r() << endl;
			}
			Line* new_line = new Line();
			current_page->add_child(new_line);
			new_line->set_prev(current_line);
			current_line = new_line;
			y_adjust += settings::line_height_;
			current_line->set_geometry(0, y_adjust, 0, settings::line_height_);
			lines_.push_back(current_line);
			last = nullptr;
			iter++;
		}
		child->set_prev(last);
		current_line->expand_box(child);
		last = child;
	}
	//cout << lines_.size() << endl;
}

void Typesetter::justify()
{
	//check eack box
	for (int i = 0; i < lines_.size(); i++)
	{
		//cout << "line: " << i << endl;
		Line* line = lines_[i];

		const vector<Item*>* children = line->children();		

		//move everything
		double adjustment = 0;

		for (Item* item : *children)
		{
			//cout << item->content() << " " << item->x() << " " << item->end_at() << " " << adjustment << endl;
			item->translate(adjustment, 0);
			if (item->type() == Item::GLUE)
			{
				double delta = line->r() * (line->r() < 0 ? item->shrinkability() : item->stretchability());
				item->set_width(item->width() + delta);
				adjustment += delta;
			}
		}
	}
}


void Typesetter::Render(RenderTarget target)
{
	Progress("Generating optput files");
	if (target == RenderTarget::SVG)
	{
		for ( int i = 0; i < pages_.size(); i++)
		{
			ofstream file;
			file.open("output/svg/page" + to_string(i) + ".svg");
			file << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
			file << "<svg\n";
			file << "	xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
			file << "	xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n";
			file << "	xmlns:cc=\"http://creativecommons.org/ns#\"\n";
			file << "	xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n";
			file << "	xmlns:svg=\"http://www.w3.org/2000/svg\"\n";
			file << "	xmlns=\"http://www.w3.org/2000/svg\"\n";
			file << "	version=\"1.1\"\n";
			file << "	viewBox=\"0 0 " << settings::mm_to_point(settings::page_width_) << " " << settings::mm_to_point(settings::page_height_) << "\">\n";
			file << "<defs>\n";

			//output cache
			for (auto& kv : glyph_cache_)
			{
				file << "<path fill='rgb(0,0,0)' id='char" << int(kv.first) << "' class='char' ";
				file << "d='" << kv.second->path()->SVG() << "'";
				file << "/>\n";
			}

			file << "</defs>\n";
			file << "<g transform = \"scale(1, 1)\">\n";


			//print all the boxes
			//cout << target << " " << (target == RenderTarget::SVG_CACHE) << endl;
			pages_[i]->SVG(file);
			file << "\n";

			if (settings::show_river_)
			{
				file << "<g transform='translate(" << pages_[i]->x() << ", " << pages_[i]->y() << ")'>\n";
				for (River* river : rivers_[i])
				{
					river->Analyse();
					river->SVG(file);
				}
				file << "</g>\n";
			}

			file << "</g>\n";
			file << "</svg>\n";
			file.close();
		}

		//output river xml
		if (settings::show_river_)
		{
			ofstream file;
			file.open("output/script/rivers.js");
			file << "rivers = [];\n";
			for (vector<River*> rivers : rivers_)
			{
				for (River* river : rivers)
				{
					file << "rivers.push({id:" << river->id() << ", page:" << river->page() << ", size:" << river->size() << ", local:" << river->local_deviation_ << ", global:" << river->global_deviation_;
					file << ", width_std:" << river->width_std_;
					file << ", min_left_length:" << river->min_left_length_;
					file << ", min_right_length:" << river->min_right_length_;
					file << ", left_length_bar:" << river->left_length_bar_;
					file << ", right_length_bar:" << river->right_length_bar_;
					file << ", up_length:" << river->up_length_;
					file << ", down_length:" << river->down_length_;
					file << ", volume:" << river->volume_;
					file << ", repeats:" << river->repeats_;
					file << ", repeat_words:" << river->repeat_words_;
					file << ", repeat_word:'" << river->repeat_word_ << "'";
					file << "});\n";
				}
			}
			file.close();
		}
	}


	Progress("Done");
}