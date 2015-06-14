#include "typesetter.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
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
		message("An error occurred during FT library initialization.");
	}

	std::string font_pos = "./Fonts/" + settings::font_;
	const char* font_pos_c = font_pos.c_str();
	error = FT_New_Face(library_, font_pos_c, 0, &face_);
	if (error == FT_Err_Unknown_File_Format)
	{
		message("This font format is unsupported.");
	}
	else if (error)
	{
		message("Unable to read the font file or it is broken.");
	}

	error = FT_Set_Char_Size(face_, 0, settings::font_size_ * 64, 0, 72);
	if (error)
	{
		message("Cannot set char size");
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
		message("Error loading character");
	}

	settings::space_width_ = face_->glyph->advance.x;
	settings::stretchability_ = settings::space_width_ / 3;
	settings::shrinkability_ = settings::space_width_ / 3;
}

Typesetter::~Typesetter()
{
}

void Typesetter::message(const string& msg)
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

Item* Typesetter::insert_hyphen(string hyphenated, Item* last)
{
	return last;
	//cout << hyphenated << endl;
	auto iter = paragraph_.rbegin();
	for (int i = hyphenated.length() - 1; i >= 0; i--)
	{
		if (hyphenated[i] != '-')
		{
			iter++;
			continue;
		}
		Penalty* penalty;
		//TODO::is that the correct penalty?
		if ((*iter)->glyph() == hyphen_glyph_)
		{
			//TODO::Bug with BOOK SIX: 1808 - 10
			penalty = new Penalty(0);
			penalty->set_geometry((*iter)->end_at(), 0, 0, 0);
			penalty->sum_y_ = (*iter)->sum_y_;
			penalty->sum_z_ = (*iter)->sum_z_;
			penalty->sum_y_font_ = (*iter)->sum_y_font_;
			penalty->sum_z_font_ = (*iter)->sum_z_font_;
			penalty->set_prev((*iter));
			penalty->set_glyph(nullptr);
			paragraph_.insert(iter.base(), penalty);
			iter++;
			iter++;
		}
		else
		{
			penalty = new Penalty(50);
			penalty->set_geometry((*iter)->end_at(), hyphen_glyph_->hori_bearing_y(), hyphen_width_, hyphen_glyph_->height());
			penalty->sum_y_ = (*iter.base())->sum_y_;
			penalty->sum_z_ = (*iter.base())->sum_z_;
			penalty->sum_y_font_ = (*iter.base())->sum_y_font_;
			penalty->sum_z_font_ = (*iter.base())->sum_z_font_;
			penalty->set_prev((*iter));
			penalty->set_glyph(hyphen_glyph_);
			paragraph_.insert(iter.base(), penalty);
			iter++;
		}

		//insert at last
		if ((*iter) == last)
		{
			last = penalty;
		}
	}
	return last;
}

void Typesetter::Typeset()
{
	start_time_ = chrono::high_resolution_clock::now();

	Hyphenator hyphenator = (RFC_3066::Language("en"));

	FT_Error error = FT_Load_Char(face_, '-', FT_LOAD_NO_SCALE);
	if (error)
	{
		message("Error loading character");
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
	unsigned long long sum_y_font = 0;
	unsigned long long sum_z_font = 0;

	Item* last_item = nullptr;
	Word* word = nullptr;
	wchar_t last_ch = 0;
	wchar_t ch = 0;
	wchar_t next_ch;
	//fstream fin(file_, fstream::in);
	//TODO::Understand it....
	std::wifstream fin(file_, std::ios::binary);
	fin.imbue(std::locale(fin.getloc(),
		new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	bool new_paragraph = true;

	while (true)
	{
		fin.get(next_ch);
		ch = next_ch;
		//cout << char(ch) << " - " << int(ch) << endl;
		//TODO::What is this...
		if (int(ch) == 13)
		{			
			continue;
		}

		//new line indent
		if (new_paragraph)
		{
			new_paragraph = false;
			Box* box = new Box(nullptr, nullptr);
			box->set_geometry(x_cursor, 0, 2 * settings::em_size_, 0);
			x_cursor += box->width();
			paragraph_.push_back(box);
			box->sum_y_ = sum_y;
			box->sum_z_ = sum_z;
			box->sum_y_font_ = sum_y_font;
			box->sum_z_font_ = sum_z_font;
			box->set_prev(last_item);
			last_item = box;
		}

		//backspace
		if (ch == '\n' || fin.eof())
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator);
				last_item = insert_hyphen(word->hyphenated(), last_item);
			}
			word = nullptr;
			//must break
			Glue* glue = new Glue(0, 1000000, 0);
			glue->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(glue);
			glue->sum_y_ = sum_y;
			glue->sum_z_ = sum_z;
			glue->sum_y_font_ = sum_y_font;
			glue->sum_z_font_ = sum_z_font;
			sum_y += glue->stretchability();
			sum_z += glue->shrinkability();
			glue->set_prev(last_item);
			Penalty* penalty = new Penalty(-1000);
			penalty->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(penalty);
			penalty->sum_y_ = sum_y;
			penalty->sum_z_ = sum_z;
			penalty->sum_y_font_ = sum_y_font;
			penalty->sum_z_font_ = sum_z_font;
			penalty->set_prev(glue);
			last_ch = ch;
			last_item = penalty;
			break_paragraph();
			sum_y = 0; 
			sum_z = 0;
			sum_y_font = 0;
			sum_z_font = 0;
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
				last_item = insert_hyphen(word->hyphenated(), last_item);
			}
			word = nullptr;
			Glue* glue = new Glue(settings::space_width_, settings::stretchability_, settings::shrinkability_);
			glue->set_geometry(x_cursor, 0, settings::space_width_, 0);
			paragraph_.push_back(glue);
			glue->sum_y_ = sum_y;
			glue->sum_z_ = sum_z;
			glue->sum_y_font_ = sum_y_font;
			glue->sum_z_font_ = sum_z_font;
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
					message("Error loading character");
				}

				glyph = new Glyph(face_->glyph, ch);
				glyph_cache_[ch] = glyph;
			}
			else
			{
				glyph = cache_index->second;
			}

			Box* box = new Box(glyph, word);
			box->set_geometry(x_cursor, box->glyph()->hori_bearing_y(), glyph->advance().x(), -box->glyph()->height());
			paragraph_.push_back(box);
			box->sum_y_ = sum_y;
			box->sum_z_ = sum_z;
			box->sum_y_font_ = sum_y_font;
			box->sum_z_font_ = sum_z_font;
			box->set_prev(last_item);
			sum_y += glyph->width() * settings::max_expansion_;
			sum_z += glyph->width() * settings::max_expansion_;
			sum_y_font += glyph->width() * settings::max_expansion_;
			sum_z_font += glyph->width() * settings::max_expansion_;
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
	fill_lines();
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
				Box* up = nullptr;
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

				if (up != nullptr)
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



void Typesetter::fill_lines()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Page* current_page = new Page();
	current_page->set_geometry(settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_), settings::content_width_point(), settings::content_height_point());
	pages_.push_back(current_page);
	Line* current_line = new Line();
	current_line->set_geometry(0, y_adjust, settings::content_width_point(), settings::line_height_);
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
				current_line->set_demerits((*iter)->demerits());
				//cout << breakpoint->r() << endl;
			}
			Line* new_line = new Line();
			current_page->add_child(new_line);
			new_line->set_prev(current_line);
			current_line = new_line;
			y_adjust += settings::line_height_;
			current_line->set_geometry(0, y_adjust, settings::content_width_point(), settings::line_height_);
			lines_.push_back(current_line);
			last = nullptr;
			iter++;
		}
		child->set_prev(last);
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
		//why no children?
		if (children->size() == 0)
			continue;

		//font expansion, shinkability and stretchability created by font.
		long long y = children->back()->sum_y_ - children->front()->sum_y_;
		long long z = children->back()->sum_z_ - children->front()->sum_z_;
		long long font_y = children->back()->sum_y_font_ - children->front()->sum_y_font_;
		long long font_z = children->back()->sum_z_font_ - children->front()->sum_z_font_;

		double space_r, font_r; //ratio for space and fonts
		if (settings::expansion_mode_ == 0)
		{
			// font first
			long long diff = -line->demerits().length + settings::content_width_point();
			if (diff > 0)
			{
				//stretch
				long long font_partition = font_y >= diff ? diff : font_y;
				font_r = 1.0f * font_partition / font_y;
				space_r = 1.0f * (diff - font_partition) / (y - font_y);
				//cout << diff << " " << font_partition << " " << font_y << " " << y << " " << space_r << " " << font_r << endl;
				//cout << children->back()->sum_y_font_ << " " << children->front()->sum_y_font_ << endl;
				//cout << line->demerits().length << " " << diff << " " << font_partition << endl;
			}
			else
			{
				//shrink
				diff = -diff;
				long long font_partition = font_z >= diff ? diff : font_z;
				font_r = -1.0f * font_partition / font_z;
				space_r = -1.0f * (diff - font_partition) / (z - font_z);
			}
		}
		else
		{
			//uniform
			space_r = line->demerits().r;
			font_r = line->demerits().r;
		}

		//move everything
		double adjustment = 0;
		line->set_r(space_r, font_r);

		for (Item* item : *children)
		{
			//cout << item->content() << " " << item->x() << " " << item->end_at() << " " << adjustment << endl;
			item->translate(adjustment, 0);
			if (item->type() == Item::GLUE)
			{
				double delta = space_r * (space_r < 0 ? item->shrinkability() : item->stretchability());
				item->set_width(item->width() + delta);
				adjustment += delta;
			}
			if (item->glyph() != nullptr)
			{
				//font expansion
				item->set_expansion(font_r * settings::max_expansion_ + 1);
				double delta = font_r * settings::max_expansion_ * item->glyph()->width();
				item->set_width(item->width() + delta);
				adjustment += delta;
			}
		}
	}

}

void Typesetter::render_page(RenderTarget target, int page)
{
	if (page < 0 || page >= pages_.size())
	{
		message("Error: Illegal page number " + to_string(page));
		return;
	}
	ofstream file;
	file.open("output/svg/page" + to_string(page) + ".svg");
	file << fixed << setprecision(4);
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
	pages_[page]->SVG(file);
	file << "\n";

	if (settings::show_river_)
	{
		file << "<g transform='translate(" << pages_[page]->x() << ", " << pages_[page]->y() << ")'>\n";
		for (River* river : rivers_[page])
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

void Typesetter::render(RenderTarget target)
{
	Progress("Generating optput files");
	if (target == RenderTarget::SVG)
	{
		for ( int i = 0; i < pages_.size(); i++)
		{
			render_page(target, i);
		}
		//meta data
		ofstream file;
		file.open("output/script/meta.js");
		file << "PAGE_COUNT = " << pages_.size() << ";\n";
		file << "FONT_SIZE = '" << settings::font_size_ << "';\n";
		file << "FONT = '" << settings::font_ << "';\n";
		file.close();

		//output river 
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