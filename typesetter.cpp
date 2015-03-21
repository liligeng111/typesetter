#include "typesetter.h"
#include <iostream>
#include <fstream>

Typesetter::Typesetter()
{
	content_ = "abc";
	//content_ = "\"The quick brown fox jumps over the lazy dog\" is an English-language pangram¡ªa phrase that contains all of the letters of the alphabet. It is commonly used for touch-typing practice. It is also used to test typewriters and computer keyboards, show fonts, and other applications involving all of the letters in the English alphabet. Owing to its brevity and coherence, it has become widely known.";
	
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

	Render(RenderTarget::SVG);
}

Typesetter::~Typesetter()
{
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
		file << "	viewBox=\"0 0 3000 2000\">\n";
		file << "<defs>\n";
		file << "</defs>\n";


		file << "</g>\n";
		file << "</svg>\n";
		file.close();
	}
}