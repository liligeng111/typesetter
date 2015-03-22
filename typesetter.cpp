#include "typesetter.h"
#include <iostream>
#include <fstream>
#include "viewer.h"

Typesetter::Typesetter()
{
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

	error = FT_Set_Char_Size(face_, 0, 12 * 64, 72, 72);
	if (error)
	{
		Message("Cannot set char size");
	}
	error = FT_Set_Pixel_Sizes(face_, 0, 64);
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

void Typesetter::Typeset()
{
	//clear previous boxes
	clean();

	long x_cursor = 0;
	long y_cursor = 0;
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
		
		Glyph* glyph = new Glyph(face_->glyph);
		Box* box = new Box(glyph);

		box->set_x(x_cursor);
		boxes_.push_back(*box);
		x_cursor += box->glyph()->advance().x();
		//x_cursor += face_->glyph->metrics.width;
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
		file << "	viewBox=\"0 0 20000 30000\">\n";
		file << "<defs>\n";
		file << "</defs>\n";
		file << "<g transform = \"translate(-15000, 5000) scale(1, -1)\">\n";


		//print all the boxes
		int i = 1;
		for (Box& box : boxes_)
		{
			file << box.SVG(i) << endl;
			i++;
		}

		file << "</g>\n";
		file << "</svg>\n";
		file.close();
	}
}