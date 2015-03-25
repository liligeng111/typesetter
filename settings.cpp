#include "settings.h"

namespace settings
{
	float paper_width_ = 210;
	float paper_height_ = 297;
	float margin_top_ = 19;
	float margin_left_ = 19;
	float margin_right_ = 13.2;
	float margin_bottom_ = 36.7;
	
	int dpi_ = 96;

	int font_size_ = 12;

	//sapce, char, word, line, page
	bool border_[] = { true, false, true, true, true };
	std::string border_color[] = { "(15, 250, 0)", "(250, 15, 0)", "(250, 150, 0)", "(120, 120, 0)", "(0, 150, 200)" };
};