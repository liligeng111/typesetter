#include "settings.h"

namespace settings
{
	float page_width_ = 210;
	float page_height_ = 297;
	float margin_top_ = 19;
	float margin_left_ = 19;
	float margin_right_ = 13.2;
	float margin_bottom_ = 36.7;
	
	int dpi_ = 96;

	int font_size_ = 12;
	AlignMode align_mode_ = settings::FIRST_FIT;
	long space_width_ = 0;
	long stretchability_ = 0;
	long shrinkability_ = 0;

	//sapce, char, word, line, page
	bool border_[] = { false, false, false, false, false };
	std::string border_color[] = { "(25, 50, 150)", "(250, 15, 0)", "(250, 150, 0)", "(120, 120, 0)", "(0, 150, 200)" };

	bool show_river_ = false;


	float AdjustmentRatio(long L, long l)
	{
		float r;
		if (L == l)
			r = 0;
		else if (L < l)
			r = (l - L) * 1.0f / settings::stretchability_;
		else
			r = (l - L) * 1.0f / settings::shrinkability_;
		return r;
	}
};