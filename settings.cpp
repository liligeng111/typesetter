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

	int font_size_ = 12;
	AlignMode align_mode_ = settings::BEST_FIT;
	long em_size_ = 2048;
	long line_height_ = 2048;
	long space_width_ = 0;
	long stretchability_ = 0;
	long shrinkability_ = 0;

	//sapce, char, word, line, page
	bool border_[] = { false, false, false, false, false };
	std::string border_color[] = { "(25, 50, 150)", "(250, 15, 200)", "(250, 150, 0)", "(120, 120, 0)", "(0, 150, 200)" };

	bool show_river_ = false;
	float river_threshold_ = 2.0f;


	float AdjustmentRatio(long L, long l, int n)
	{
		float r;
		if (L == l)
			r = 0;
		else if (L < l)
			r = (l - L) * 1.0f / (settings::stretchability_ * n);
		else
			r = (l - L) * 1.0f / (settings::shrinkability_ * n);
		return r;
	}
};