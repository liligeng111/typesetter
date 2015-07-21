#include "settings.h"

namespace settings
{
	float page_width_ = 210;
	//temp
	float page_height_ = 297;
	float margin_top_ = 19;
	float margin_left_ = 19;
	float margin_right_ = 13.2;
	float margin_bottom_ = 36.7;
	
	int dpi_ = 72; 

	std::string font_ = "times.ttf";
	int font_size_ = 24;
	long em_size_ = 2048;
	long line_height_ = 2048;
	long space_width_ = 0;
	long stretchability_ = 0;
	long shrinkability_ = 0;
	long descender_ = 0;

	//sapce, char, word, line, page
	bool border_[] = { true, true, true, true, true, true };
	int markdown_type_ = 0;

	bool show_river_ = false;
	float river_threshold_ = 2.0f;

	float rho_ = 10.0f;
	float stretch_ratio_ = 1.0f / 3;
	float shrink_ratio_ = 1.0f / 3;
	float letter_space_stretch_ratio_ = 0.05;
	float letter_space_shrink_ratio_ = 0.05;
	float max_expansion_ = 0.00;
	int expansion_mode_ = 0;



	//magic
	bool use_magic_ = false;
	float min_magic_gain_ = 10000;
	float max_magic_amount_= 1;
};