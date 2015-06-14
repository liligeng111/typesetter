#include "settings.h"

namespace settings
{
	float page_width_ = 210;
	float page_height_ = 297;
	float margin_top_ = 19;
	float margin_left_ = 19;
	float margin_right_ = 13.2;
	float margin_bottom_ = 36.7;
	
	int dpi_ = 72; 

	std::string font_ = "MinionPro-Regular.otf";
	int font_size_ = 24;
	long em_size_ = 2048;
	long line_height_ = 2048;
	long space_width_ = 0;
	long stretchability_ = 0;
	long shrinkability_ = 0;
	long descender_ = 0;

	//sapce, char, word, line, page
	bool border_[] = { true, true, true, true, true };
	std::string border_color[] = { "(25, 50, 150)", "(250, 15, 200)", "(250, 150, 0)", "(120, 120, 0)", "(0, 150, 200)" };

	bool show_river_ = false;
	float river_threshold_ = 2.0f;

	float rho_ = 5.0f;
	float max_expansion_ = 0.02;
	int expansion_mode_ = 0;

};