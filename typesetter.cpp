#include "typesetter.h"
#include <iostream>
#include <fstream>
#include "viewer.h"
#include "settings.h"

Typesetter::Typesetter()
{
	root_ = new Box(NULL);
	//just to be sure
	content_ = "";
	
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
	delete root_; 
	root_ = new Box(NULL);
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
		

		//ignore space
		if (content_[i] == ' ')
		{
			x_cursor += face_->glyph->advance.x;
			continue;
		}
		//new word
		if (i == 0 || content_[i - 1] == ' ')
		{
			current_box = new Box(root_);
			current_box->set_x(x_cursor);
		}

		Glyph* glyph = new Glyph(face_->glyph);
		Box* box = new Box(glyph, current_box);

		box->set_x(x_cursor - current_box->x());
		//not really beautiful
		current_box->set_width(x_cursor - current_box->x() + box->glyph()->advance().x());
		x_cursor += box->glyph()->advance().x();
	}

	Align();
}

void Typesetter::Align()
{
	//clear previous boxes
	long x_cursor = 0;
	long x_adjust = 0;
	long y_cursor = 0;
	long y_adjust = 0;

	//check eack box
	for (Box* child : *(root_->children()))
	{
		if (child->x() + child->width() + x_adjust > settings::content_width_char())
		{
			//need newline
			x_cursor = 0;
			x_adjust = -child->x();
			y_adjust -= 2000;
		}
		child->Transform(x_adjust, y_adjust);
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
		file << "	viewBox=\"0 0 " + to_string(settings::mm_to_char_(settings::paper_width_)) + " " + to_string(settings::mm_to_char_(settings::paper_height_)) + "\">\n";
		file << "<defs>\n";
		file << "</defs>\n";
		file << "<g transform = \"translate(" + to_string(settings::mm_to_char_(settings::margin_left_)) + ", " + to_string(settings::mm_to_char_(settings::margin_top_))+ ") scale(1, -1)\">\n";


		//print all the boxes
		file << root_->SVG() << endl;

		file << "</g>\n";
		file << "</svg>\n";
		file.close();
	}
}