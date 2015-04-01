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

	root_ = new Box(NULL, Box::BoxType::PAGE);
	root_->set_geometry(settings::mm_to_fixed_point(settings::margin_left_), settings::mm_to_fixed_point(settings::margin_top_), settings::content_width_fixed_point(), settings::content_height_fixed_point());
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
	root_->Clear();
	words_ = vector<Box*>();
	lines_ = vector<Box*>();
	rivers_ = vector<River*>();
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
	for (int i = 1; i < lines_.size() - 1; i++)
	{
		for (Box* box : *(lines_[i]->children()))
		{
			if (box->type() != Box::BoxType::SPACE)
				continue;	
			Box* up = NULL;
			Box* down = NULL;
			for (Box* temp : *(lines_[i - 1]->children()))
			{
				if (temp->type() != Box::BoxType::SPACE)
					continue;
				if (labs(box->MidPoint().x() - temp->MidPoint().x()) < 2 * box->width())
				{
					up = temp;
					break;
				}
			}

			for (Box* temp : *(lines_[i + 1]->children()))
			{
				if (temp->type() != Box::BoxType::SPACE)
					continue;
				if (labs(box->MidPoint().x() - temp->MidPoint().x()) < 2 * box->width())
				{
					down = temp;
					break;
				}
			}
			if (up == NULL || down == NULL)
				continue;
			River* river = new River();
			rivers_.push_back(river);
			river->AddBox(box);
			river->AddBox(up);
			river->AddBox(down);
		}
	}
}

void Typesetter::RaggedRight()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Box* current_line = new Box(root_, Box::BoxType::LINE);
	current_line->set_geometry(0, y_adjust, 0, line_height_);
	lines_.push_back(current_line);

	//check each box
	for (Box* child : words_)
	{
		if (child->type() == Box::BoxType::BACKSPACE || (child->type() == Box::BoxType::WORD && child->x() + child->width() + x_adjust > settings::content_width_fixed_point()))
		{
			//need newline
			current_line = new Box(root_, Box::BoxType::LINE);
			x_adjust = -child->x();
			y_adjust += line_height_;
			current_line->set_geometry(0, y_adjust, 0, line_height_);
			lines_.push_back(current_line);
		}
		child->Translate(x_adjust, 0);
		child->set_parent(current_line);
		current_line->ExpandBox(child);
	}

}

void Typesetter::FirstFit()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Box* current_line = new Box(root_, Box::BoxType::LINE);
	current_line->set_geometry(0, y_adjust, 0, line_height_);
	lines_.push_back(current_line);

	//check each box
	for (Box* child : words_)
	{
		if (child->type() == Box::BoxType::BACKSPACE || (child->type() == Box::BoxType::WORD && child->x() + child->width() / 2 + x_adjust > settings::content_width_fixed_point()))
		{
			//need newline
			current_line = new Box(root_, Box::BoxType::LINE);
			x_adjust = -child->x();
			y_adjust += line_height_;
			current_line->set_geometry(0, y_adjust, 0, line_height_);
			lines_.push_back(current_line);
		}
		child->Translate(x_adjust, 0);
		child->set_parent(current_line);
		current_line->ExpandBox(child);
	}

	Justify();
}

void Typesetter::BestFit()
{
	long x_adjust = 0;
	long y_adjust = 0;
	Box* current_line = new Box(root_, Box::BoxType::LINE);
	current_line->set_geometry(0, y_adjust, 0, line_height_);
	lines_.push_back(current_line);

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
			long L = child->x() + child->width() + x_adjust;
			long l = settings::content_width_fixed_point();

			float r = settings::AdjustmentRatio(L, l);
			//if is a possible breakpoint, check for better
			if (r <= 100 && r >= -100)
			{
				new_line = true;
				float beta = 100 * r * r * abs(r);

				for (int j = i + 1; r > 0 && j < words_.size(); j++)
				{
					L = words_[j]->x() + words_[j]->width() + x_adjust;
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
			//need newline
			current_line = new Box(root_, Box::BoxType::LINE);
			x_adjust = -child->x();
			y_adjust += line_height_;
			current_line->set_geometry(0, y_adjust, 0, line_height_);
			lines_.push_back(current_line);
		}
		child->Translate(x_adjust, 0);
		child->set_parent(current_line);
		current_line->ExpandBox(child);
	}

	Justify();
}

void Typesetter::Justify()
{
	//check eack box
	for (int i = 0; i < lines_.size(); i++)
	{
		Box* line = lines_[i];
		//skip last line
		if (i == lines_.size() - 1 || lines_[i + 1]->width() == 0)
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
		ofstream file;
		file.open("output.svg");
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
		file << root_->SVG() << endl;

		if (settings::show_river_)
		{
			file << "<g transform='translate(" + to_string(root_->x()) + ", " + to_string(root_->y()) + ")'>\n";
			for (River* river : rivers_)
			{
				file << river->SVG() << endl;
			}
			file << "</g>\n";
		}

		file << "</g>\n";
		file << "</svg>\n";
		file.close();
	}
}