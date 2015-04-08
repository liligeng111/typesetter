#include "typesetter.h"
#include <iostream>
#include <fstream>
#include "viewer.h"
#include "settings.h"

Typesetter::Typesetter()
{
	//just to be sure
	content_ = "";	
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

	error = FT_New_Face(library_, "./fonts/times.ttf", 0, &face_);
	if (error == FT_Err_Unknown_File_Format)
	{
		Message("This font format is unsupported.");
	}
	else if (error)
	{
		Message("Unable to read the font file or it is broken.");
	}

	error = FT_Set_Char_Size(face_, 0, settings::font_size_ * 64, 0, settings::dpi_);
	if (error)
	{
		Message("Cannot set char size");
	}
	// according to http://www.freetype.org/freetype2/docs/glyphs/glyphs-2.html
	error = FT_Set_Pixel_Sizes(face_, 0, settings::font_size_ * settings::dpi_ / 72);
	if (error)
	{
		Message("Cannot set pixel size");
	}

	Box::set_descender(face_->descender);
	line_height_ = face_->size->metrics.y_ppem * 64;


	settings::space_width_ = face_->size->metrics.x_ppem * 64 / 3;
	settings::stretchability_ = face_->size->metrics.x_ppem * 64 / 6;
	settings::shrinkability_ = face_->size->metrics.x_ppem * 64 / 9;
}

Typesetter::~Typesetter()
{
}

void Typesetter::Message(const string& msg)
{ 
	Viewer::Message(msg); 
}

void Typesetter::Clean()
{
	words_ = vector<Box*>();
	lines_ = vector<Box*>();
	pages_ = vector<Box*>();
	rivers_ = vector<vector<River*>>();
	breakpoints_ = vector<Breakpoint*>();
}

void Typesetter::Typeset()
{
	//clear previous boxes
	Clean();

	Box* current_box;
	long x_cursor = 0;
	FT_UInt last_char = 0;

	for (int i = 0; i < content_.length(); i++)
	{
		FT_Error error = FT_Load_Char(face_, content_[i], FT_LOAD_DEFAULT);
		if (error)
		{
			Message("Error loading character");		
		}
		
		//kern
		FT_Vector* kern = new FT_Vector();
		//? what are the modes?
		FT_Get_Kerning(face_, last_char, content_[i], FT_KERNING_DEFAULT, kern);
		x_cursor += kern->x;
		last_char = content_[i];
		
		//backspace
		if (content_[i] == '\n')
		{
			current_box = new Box(NULL, Box::BoxType::BACKSPACE);
			current_box->set_geometry(x_cursor, 0, 0, 0);
			words_.push_back(current_box);
			continue;
		}

		//new space
		if (content_[i] == ' ' && (i == 0 || content_[i - 1] != ' '))
		{
			current_box = new Box(NULL, Box::BoxType::SPACE);
			current_box->set_geometry(x_cursor, 0, 0, 0);
			words_.push_back(current_box);
		}
		//new word
		else if (content_[i] != ' ' && (i == 0 || content_[i - 1] == ' '))
		{
			current_box = new Box(NULL, Box::BoxType::WORD);
			current_box->set_geometry(x_cursor, 0, 0, 0);
			words_.push_back(current_box);
		}

		Glyph* glyph = new Glyph(face_->glyph);
		Box* box;
		// noly add char
		if (content_[i] == ' ')
		{
			box = new Box(glyph, NULL, Box::BoxType::CHAR);
			box->set_geometry(x_cursor - current_box->x(), 0, settings::space_width_, box->glyph()->advance().y());
			x_cursor += settings::space_width_;
		}
		else
		{
			box = new Box(glyph, current_box, Box::BoxType::CHAR);
			box->set_geometry(x_cursor - current_box->x() + box->glyph()->hori_bearing_x(), box->glyph()->hori_bearing_y(), box->glyph()->width(), -box->glyph()->height());
			x_cursor += box->glyph()->advance().x();
		}

		current_box->ExpandBox(box);
	}

	if (settings::align_mode_ == settings::AlignMode::RAGGED_RIGHT)
	{
		RaggedRight();
	}
	else if (settings::align_mode_ == settings::AlignMode::FIRST_FIT)
	{
		FirstFit();
	}
	else if (settings::align_mode_ == settings::AlignMode::BEST_FIT)
	{
		BestFit();
	}

	if (settings::show_river_)
	{
		DetectRiver();
	}
}

void Typesetter::DetectRiver()
{
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
				if (box->type() != Box::BoxType::SPACE || box == box->parent()->child(box->parent()->ChildrenSize() - 1))
					continue;	
				
				//check up for duplicate
				Box* up = NULL;
				for (int t = 0; i > 0 && t < lines->at(i - 1)->ChildrenSize(); t++)
				{
					Box* temp = lines->at(i - 1)->child(t);
					if (temp->type() != Box::BoxType::SPACE || temp == temp->parent()->child(temp->parent()->ChildrenSize() - 1))
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
						if (temp->type() != Box::BoxType::SPACE || temp == temp->parent()->child(temp->parent()->ChildrenSize() - 1))
							continue;
						if (labs(last->MidPoint().x() - temp->MidPoint().x()) < settings::river_threshold_ * settings::space_width_)
						{
							last = temp;
							found = true;
							break;
						}
					}
				}

				if (river->size() > 2)
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
}

void Typesetter::RaggedRight()
{
	Breakpoint* breakpoint = new Breakpoint(words_[0]);
	breakpoints_.push_back(breakpoint);
	//check each box
	for (Box* child : words_)
	{
		if (child->type() == Box::BoxType::BACKSPACE || (child->type() == Box::BoxType::WORD && child->EndAt() -  breakpoint->x()> settings::content_width_fixed_point()))
		{
			//break line
			breakpoint = new Breakpoint(child);
			breakpoints_.push_back(breakpoint);
		}
	}

	BreakLines();
}

void Typesetter::FirstFit()
{
	Breakpoint* breakpoint = new Breakpoint(words_[0]);
	breakpoints_.push_back(breakpoint);
	//check each box
	for (Box* child : words_)
	{
		if (child->type() == Box::BoxType::BACKSPACE || (child->type() == Box::BoxType::WORD && child->EndAt() -  breakpoint->x()> settings::content_width_fixed_point()))
		{
			//break line
			breakpoint = new Breakpoint(child);
			breakpoints_.push_back(breakpoint);
		}
	}
	
	BreakLines();
	Justify();
}

void Typesetter::BestFit()
{
	Breakpoint* breakpoint = new Breakpoint(words_[0]);
	breakpoints_.push_back(breakpoint);

	//check each box
	for (int i = 0; i < words_.size(); i++)
	{
		if (i == words_.size() - 3)
		{
			int t = 0;
		}

		Box* child = words_[i];
		bool new_line = child->type() == Box::BoxType::BACKSPACE;
		//calculate r
		if (!new_line && child->type() == Box::BoxType::WORD)
		{
			long L = child->EndAt() -  breakpoint->x();
			long l = settings::content_width_fixed_point();

			float r = settings::AdjustmentRatio(L, l);
			//if is a possible breakpoint, check for better
			if (r <= 100 && r >= -100)
			{
				new_line = true;
				float beta = 100 * r * r * abs(r);

				for (int j = i + 1; r > 0 && j < words_.size(); j++)
				{
					L = words_[j]->EndAt() -  breakpoint->x();
					r = settings::AdjustmentRatio(L, l);
					float temp = 100 * r * r * abs(r);
					//found better
					if (temp < beta)
					{
						new_line = false;
						break;
					}
				}

			}
		}

		if (new_line)
		{
			//break line
			breakpoint = new Breakpoint(child);
			breakpoints_.push_back(breakpoint);
		}
	}
	
	BreakLines();
	Justify();
}

void Typesetter::BreakLines()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Box* current_line;
	Box* current_page = new Box(NULL, Box::BoxType::PAGE);
	current_page->set_geometry(settings::mm_to_fixed_point(settings::margin_left_), settings::mm_to_fixed_point(settings::margin_top_), settings::content_width_fixed_point(), settings::content_height_fixed_point());
	pages_.push_back(current_page);

	int i = 0;

	//check each box, first must be a breakpoint
	for (Box* child : words_)
	{
		if (i < breakpoints_.size() && child == breakpoints_[i]->box())
		{
			//need newline
			//new page
			if (y_adjust + line_height_ > settings::content_height_fixed_point())
			{
				y_adjust = 0;
				current_page = new Box(NULL, Box::BoxType::PAGE);
				current_page->set_geometry(settings::mm_to_fixed_point(settings::margin_left_), settings::mm_to_fixed_point(settings::margin_top_), settings::content_width_fixed_point(), settings::content_height_fixed_point());
				pages_.push_back(current_page);
			}			
			i++;
			//do not jusify before backspace
			if (child->type() == Box::BoxType::BACKSPACE)
			{
				current_line->set_justify(false);
			}
			current_line = new Box(current_page, Box::BoxType::LINE);
			current_line->set_geometry(0, y_adjust, 0, line_height_);
			x_adjust = -child->x();
			y_adjust += line_height_;
			lines_.push_back(current_line);
		}
		child->Translate(x_adjust, 0);
		child->set_parent(current_line);
		current_line->ExpandBox(child);
	}
}

void Typesetter::Justify()
{
	//check eack box
	for (int i = 0; i < lines_.size(); i++)
	{
		Box* line = lines_[i];
		//skip special lines
		if (!lines_[i]->justify())
			continue;

		const vector<Box*>* children = line->children();
		Box* last = NULL;
		
		//count space
		int count = 0;
		for (int i = children->size() - 1; i >= 0; i--)
		{
			Box* box = (*children)[i];
			if (box->type() == Box::BoxType::SPACE && last != NULL)
			{
				count++;
			}
			else if (box->type() == Box::BoxType::WORD && last == NULL)
			{
				last = box;
			}
		}
		//no space
		if (count == 0)
			continue;
		float extra = settings::content_width_fixed_point() - last->width() - last->x();


		//move everything
		long delta = extra / count;
		float adjustment = 0;
		for (Box* box : *children)
		{
			if (box->type() == Box::BoxType::SPACE)
			{
				box->Translate(adjustment, 0);
				box->set_width(box->width() + delta);
				adjustment += delta;
			}
			else if (box->type() == Box::BoxType::WORD)
			{
				box->Translate(adjustment, 0);
			}
		}
	}
}


void Typesetter::Render(RenderTarget target)
{
	if (target == RenderTarget::SVG)
	{
		for ( int i = 0; i < pages_.size(); i++)
		{
			ofstream file;
			file.open("output/page" + to_string(i) + ".svg");
			file << "<?xml version=\"1.0\" standalone=\"no\"?>\n";
			file << "<svg\n";
			file << "	xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n";
			file << "	xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n";
			file << "	xmlns:cc=\"http://creativecommons.org/ns#\"\n";
			file << "	xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n";
			file << "	xmlns:svg=\"http://www.w3.org/2000/svg\"\n";
			file << "	xmlns=\"http://www.w3.org/2000/svg\"\n";
			file << "	version=\"1.1\"\n";
			file << "	viewBox=\"0 0 " + to_string(settings::mm_to_fixed_point(settings::page_width_)) + " " + to_string(settings::mm_to_fixed_point(settings::page_height_)) + "\">\n";
			file << "<defs>\n";
			file << "</defs>\n";
			file << "<g transform = \"scale(1, 1)\">\n";


			//print all the boxes
			file <<pages_[i]->SVG() << endl;

			if (settings::show_river_)
			{
				file << "<g transform='translate(" + to_string(pages_[i]->x()) + ", " + to_string(pages_[i]->y()) + ")'>\n";
				for (River* river : rivers_[i])
				{
					river->Analyse();
					file << river->SVG() << endl;
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
					file << "rivers.push({id:" + to_string(river->id()) + ", page:" + to_string(river->page()) + ", size:" + to_string(river->size()) + ", local:" + to_string(river->local_deviation()) + ", global:" + to_string(river->global_deviation()) + "});\n";
				}
			}
			file.close();
		}
	}
}