#include "typesetter.h"
#include <iostream>
#include <fstream>
#include "viewer.h"
#include "settings.h"

Typesetter::Typesetter()
{
	//just to be sure
	content_ = "";
	
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
	root_->set_geometry(settings::mm_to_char(settings::margin_left_), settings::mm_to_char(settings::margin_top_), settings::content_width_char(), settings::content_height_char());
	Box::set_descender(face_->descender);
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
	root_->Clear();
	words_ = vector<Box*>();
}

void Typesetter::Typeset()
{
	//clear previous boxes
	clean();

	Box* current_box;
	long x_cursor = 0;
	FT_UInt last_char = 0;

	for (int i = 0; i < content_.length(); i++)
	{
		FT_Error error = FT_Load_Char(face_, content_[i], FT_LOAD_RENDER);
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
		

		//new space
		if (content_[i] == ' ' && (i == 0 || content_[i - 1] != ' '))
		{
			current_box = new Box(NULL, Box::BoxType::SPACE);
			current_box->set_geometry(x_cursor, 0, 0, -face_->units_per_EM);
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
			box->set_geometry(x_cursor - current_box->x(), 0, box->glyph()->advance().x(), box->glyph()->advance().y());
		}
		else
		{
			box = new Box(glyph, current_box, Box::BoxType::CHAR);
			box->set_geometry(x_cursor - current_box->x() + box->glyph()->hori_bearing_x(), box->glyph()->hori_bearing_y(), box->glyph()->width(), -box->glyph()->height());
		}

		current_box->ExpandBox(box);
		x_cursor += box->glyph()->advance().x();
	}

	Align();
}

void Typesetter::Align()
{
	//clear previous boxes
	long x_adjust = 0;
	long y_adjust = 0;
	Box* current_line = new Box(root_, Box::BoxType::LINE);
	current_line->set_geometry(0, y_adjust, 0, face_->units_per_EM);

	//check eack box
	for (Box* child : words_)
	{
		if (child->x() + child->width() + x_adjust > settings::content_width_char())
		{
			//need newline
			current_line = new Box(root_, Box::BoxType::LINE);
			x_adjust = -child->x();
			y_adjust += face_->units_per_EM;
			current_line->set_geometry(0, y_adjust, 0, face_->units_per_EM);
		}
		child->Translate(x_adjust, 0);
		child->set_parent(current_line);
		current_line->ExpandBox(child);
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
		file << "	viewBox=\"0 0 " + to_string(settings::mm_to_char(settings::page_width_)) + " " + to_string(settings::mm_to_char(settings::page_height_)) + "\">\n";
		file << "<defs>\n";
		file << "</defs>\n";
		file << "<g transform = \"scale(1, 1)\">\n";


		//print all the boxes
		file << root_->SVG() << endl;

		file << "</g>\n";
		file << "</svg>\n";
		file.close();
	}
}