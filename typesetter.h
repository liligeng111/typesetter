#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

class Typesetter
{

public:
	enum RenderTarget
	{
		SVG = 0
	};

	Typesetter();
	~Typesetter();
	void Render(RenderTarget target);


private:
	string content_;
};
