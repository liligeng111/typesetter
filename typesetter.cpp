#include "typesetter.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iomanip>
#include "viewer.h"
#include "settings.h"
#include "lib\libhyphenate\Hyphenator.h"
#include "container.h"


using namespace Hyphenate;

Typesetter::Typesetter()
{
	LoadFace();
	sum_stretchability_[settings::item_priority_size_] = {};
	sum_shrinkability_[settings::item_priority_size_] = {};
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
	settings::stretchability_ = settings::space_width_ * settings::stretch_ratio_;
	settings::shrinkability_ = settings::space_width_  * settings::shrink_ratio_;
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
	lines_.clear();
	pages_.clear();
	rivers_.clear();
	breakpoints.clear();
	active_list_.clear();
	passive_list_.clear();
	magic_paragraphs_.clear();
	reset_changeability();

	qscintilla_line = 0;
	magic_line = -1;
}

Page* Typesetter::get_page_number(int qscintilla_line_number)
{
	//TODO::better way?
	for (Line* line : lines_)
	{
		//cout << line->qscintilla_line_number() << "  " << qscintilla_line_number << endl;
		if (line->qscintilla_line_number() == qscintilla_line_number)
		{
			return line->page();
		}
	}
	cout << "Page not found" << endl;
	return nullptr;
}

void Typesetter::Progress(string msg)
{
	cout << fixed << setprecision(3) << chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time_).count() / 1000.0 << "s--" << msg << "\n";
}

void Typesetter::Typeset(QString& text)
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
	hyphen_width_ = hyphen_glyph_->advance().x;
	Glyph* glyph;

	Progress("Cleaning previous data");
	//clear previous data
	clean();

	Progress("Typesetting");
	cout << "Page width: " << settings::content_width_point() << endl;

	unsigned long x_cursor = 0;

	Item* last_item = nullptr;
	Word* word = nullptr;
	FT_UInt  last_glyph = 0;
	FT_UInt glyph_index = 0;
	wchar_t ch = 0;

	bool new_paragraph = true;
	
	for (int text_index = 0; text_index < text.length(); text_index++)
	{
		//TODO::rewrite control

		ch = text.at(text_index).unicode();
		int move_forward = 0;
		if (text_index + 1 < text.length())
		{
			//double hyphen
			if (ch == 45 && text.at(text_index + 1).unicode() == 45)
			{
				ch = 8212;
				text_index++;
			}

			//force break
			//TODO::UTF8....
			if (ch == '/' && text.at(text_index + 1).unicode() == '/')
			{
				ch = 49794;
				text_index += 2;
			}
		}

		//quotation
		if (ch == 96)
		{
			ch = 8216;
		}
		else if (ch == 39)
		{
			ch = 8217;
		}
		
		//TODO::check for availbility78
		/*
		else if (ch == 'f' && look_ahead[0] == 'f')
		{
			ch = 64256;
			move_forward = 1;
		}
		else if (ch == 'f' && look_ahead[0] == 'i')
		{
			ch = 64257;
			move_forward = 1;
		}
		else if (ch == 'f' && look_ahead[0] == 'l')
		{
			ch = 64258;
			move_forward = 1;
		}

		for (int i = 0; i < move_forward; i++)
		{
			look_ahead[0] = look_ahead[1];
			fin.get(look_ahead[1]);
			if (fin.eof())
				look_ahead[1] = 0;
		}*/

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
			Item* item = new Item(Item::BOX);
			item->init_box(nullptr, nullptr, 0, 0);
			item->set_geometry(x_cursor, 0, 2 * settings::em_size_, 0);
			x_cursor += item->width();
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			item->set_prev(last_item);
			last_item = item;
		}
		
		//kern
		glyph_index = FT_Get_Char_Index(face_, ch);
		FT_Vector* kern = new FT_Vector();
		FT_Get_Kerning(face_, last_glyph, glyph_index, FT_KERNING_UNSCALED, kern);
		//if (kern->x != 0)
		//	cout << last_glyph << " " << glyph_index << " " << ch << " " << kern->x << endl;
		x_cursor += kern->x;

		//backspace
		if (ch == '\n' || ch == 0)
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator, hyphen_glyph_);
			}
			word = nullptr;
			//must break
			Item* item = new Item(Item::GLUE);
			item->init_glue(1000000, 0, settings::end_paragraph_glue_priority_);
			item->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			set_changeability(settings::end_paragraph_glue_priority_, item->stretchability(), item->shrinkability());
			item->set_prev(last_item);
			last_item = item;

			item = new Item(Item::PENALITY);
			item->init_penalty(-1000);
			item->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			item->set_prev(last_item);
			last_glyph = glyph_index;
			last_item = item;
			break_paragraph();

			reset_changeability();

			new_paragraph = true;
			if (text_index == text.length() - 1)
				break;
			continue;
		}
		//new space
		else if (ch == ' ')
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator, hyphen_glyph_);
			}
			word = nullptr; 
			Item* item = new Item(Item::GLUE);
			item->init_glue(settings::stretchability_, settings::shrinkability_, settings::glue_priority_);
			item->set_geometry(x_cursor, 0, settings::space_width_, 0);
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			set_changeability(settings::glue_priority_, item->stretchability(), item->shrinkability());
			item->set_prev(last_item);
			last_item = item;
		}
		//Force break
		//TODO::temp
		else if (ch == 49794)
		{
			if (word != nullptr)
			{
				word->hyphenate(&hyphenator, hyphen_glyph_);
			}
			word = nullptr;
			Item* item = new Item(Item::PENALITY);
			item->init_penalty(-999);
			item->set_geometry(x_cursor, 0, 0, 0);
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			item->set_prev(last_item);
			last_glyph = glyph_index;
			last_item = item;
		}
		//new char
		else
		{
			if (word == nullptr)
			{
				word = new Word;
			}
			//TODO::ligature && hyphen
			word->content()->append(1, ch);

			auto cache_index = glyph_cache_.find(ch);
			//seach cache
			if (cache_index == glyph_cache_.end())
			{
				FT_Error error = FT_Load_Glyph(face_, glyph_index, FT_LOAD_NO_SCALE);
				if (error)
				{
					cout << char(ch) << " - " << int(ch) << endl;
					message("Error loading character");
				}
				if (glyph_index == 0)
				{
					cout << "Unrecognized character:" << ch << endl;
				}

				glyph = new Glyph(face_->glyph, ch);
				glyph_cache_[ch] = glyph;
			}
			else
			{
				glyph = cache_index->second;
			}
			//letter_space
			if (last_item != nullptr && last_item->type() == Item::BOX)
			{
				long starting = last_item->end_at();
				long ending = x_cursor + glyph->hori_bearing_x();
				long width = ending - starting;
				if (width < 0)
				{
					starting = ending;
					width = 0;
				}
				Item* item = new Item(Item::LETTER_SPACE);
				item->set_geometry(starting, 0, width, settings::em_size_ / 2);
				item->init_letter_space(settings::letter_space_stretch_ratio_ * width, settings::letter_space_shrink_ratio_ * width);
				paragraph_.push_back(item);
				item->set_changeability(sum_stretchability_, sum_shrinkability_);
				item->set_prev(last_item);
				set_changeability(settings::letter_space_priority_, item->stretchability(), item->shrinkability());
				last_item = item;
			}

			Item* item = new Item(Item::BOX);
			item->init_box(glyph, word, settings::font_expansion_ * glyph->width(), settings::font_expansion_ * glyph->width());
			item->set_geometry(x_cursor + glyph->hori_bearing_x(), item->glyph()->hori_bearing_y(), glyph->width(), -item->glyph()->height());
			paragraph_.push_back(item);
			item->set_changeability(sum_stretchability_, sum_shrinkability_);
			item->set_prev(last_item);
			set_changeability(settings::box_priority_, item->stretchability(), item->shrinkability());
			last_item = item;
		}
		last_glyph = glyph_index;

		if (ch == ' ')
		{
			x_cursor += settings::space_width_;
		}
		else
		{
			x_cursor += glyph->advance().x;
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
	//because all chars in lines have negative orientation
	long y_adjust = settings::em_size_;
	int line_number = 0;
	Page* current_page = new Page();
	current_page->set_geometry(settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_), settings::content_width_point(), settings::content_height_point());
	pages_.push_back(current_page);
	Line* current_line = new Line();
	current_line->set_line_number(line_number);
	current_line->set_qscintilla_line_number(qscintilla_line);
	current_line->set_page(current_page);
	line_number++;
	current_line->set_geometry(0, y_adjust, settings::content_width_point(), settings::line_height_);
	current_page->add_child(current_line);
	lines_.push_back(current_line);

	Item* last = nullptr;
	auto iter = breakpoints.begin();
	
	//check each item
	for (Item* child : items_)
	{
		//cout << child->content() << " " << child->x() << " " << child->end_at() << " " << x_adjust << " " << child->is_magic() << endl;

		child->translate(x_adjust, 0);
		if (child->type() == Item::BOX || child->type() == Item::GLUE || child->type() == Item::LETTER_SPACE )
			current_line->add_child(child);
		if (iter != breakpoints.end() && child == (*iter)->item())
		{
			child->set_is_break(true);
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

			//cout << (*iter)->is_last() << endl;
			if ((*iter)->is_last())
			{
				qscintilla_line++; //corresponding line num in qsccintillaa
			}

			if (child->is_magic() && magic_line != qscintilla_line)
			{
				magic_paragraphs_.push_back(qscintilla_line);
				magic_line = qscintilla_line;
			}
			//cout << (*iter)->is_last()<< "   " << qscintilla_line << endl;

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
			new_line->set_line_number(line_number);
			new_line->set_qscintilla_line_number(qscintilla_line);
			new_line->set_page(current_page);
			line_number++;
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
	float priority_r[settings::item_priority_size_] = {};

	//check eack box
	for (int i = 0; i < lines_.size(); i++)
	{
		Line* line = lines_[i];
		//cout << "line: " << i << ":  " << line->demerits().l << endl;

		const vector<Item*>* children = line->children();
		//why no children?
		if (children->size() == 0)
			continue;


		long diff = line->demerits().l - line->demerits().length;
		bool stretch = diff > 0;
		diff = abs(diff);
		for (int i = 1; i < settings::item_priority_size_; i++)
		{
			if (diff == 0)
			{
				//nothing left to assign
				priority_r[i] = 0;
			}
			else
			{
				long changeability;
				Item* last = children->back();
				Item* first = children->front();
				if (stretch)
					changeability = last->sum_stretchability(i) - first->sum_stretchability(i);
				else
					changeability = last->sum_shrinkability(i) - first->sum_shrinkability(i);
				
				if (diff > changeability && i != settings::item_priority_size_ - 1)
				{
					// not enough
					priority_r[i] = 1;
					diff -= changeability;
				}
				else
				{
					priority_r[i] = (float)diff / changeability;
					diff = 0;
				}
				if (!stretch)
					priority_r[i] = -priority_r[i];
				//cout << i << " " << diff << " " << changeability << " " << priority_r[i] << " " << children->back()->sum_shrinkability(i) << " " << children->front()->sum_shrinkability(i) << endl;
			}
		}

		//move everything
		double adjustment = 0;
		line->set_r(priority_r[settings::box_priority_], priority_r[settings::glue_priority_], priority_r[settings::letter_space_priority_]);
		if (children->size() > 0 && children->at(0)->glyph() != nullptr)
			adjustment -= children->at(0)->glyph()->left_protruding() * children->at(0)->glyph()->width() / 1000;

		for (Item* item : *children)
		{

			//cout << item->content() << " " << item->x() << " " << item->end_at() << " " << adjustment << " " << item->is_magic() << endl;
			item->translate(adjustment, 0);

			//font expansion
			double delta;
			if (stretch)
			{
				delta = priority_r[item->priority()] * item->stretchability();
				item->set_expansion(priority_r[item->priority()] * item->stretch_ratio() + 1);

			}
			else
			{
				delta = priority_r[item->priority()] * item->shrinkability();
				item->set_expansion(priority_r[item->priority()] * item->shrink_ratio() + 1);
			}

			item->set_width(item->width() + delta);
			adjustment += delta;
			//cout << delta << " " << adjustment << " " << endl;
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

int Typesetter::get_prev_magic(int line)
{
	if (magic_paragraphs_.empty())
		return 0;

	for (int i = magic_paragraphs_.size() - 1; i >= 0; i--)
	{
		if (magic_paragraphs_[i] < line)
			return magic_paragraphs_[i];
	}
	return magic_paragraphs_[magic_paragraphs_.size() - 1];
}

int Typesetter::get_next_magic(int line)
{
	if (magic_paragraphs_.empty())
		return 0;

	for (int i = 0; i < magic_paragraphs_.size(); i++)
	{
		if (magic_paragraphs_[i] > line)
			return magic_paragraphs_[i];
	}
	return magic_paragraphs_[0];
}