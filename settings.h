#pragma once
#include <string>

namespace settings
{
	// in mm.
	extern float page_width_;
	extern float page_height_;
	// Diff for first page?
	extern float margin_top_;
	extern float margin_left_;
	extern float margin_right_;
	extern float margin_bottom_;

	extern int dpi_;

	extern int font_size_;
	enum AlignMode
	{
		RAGGED_RIGHT = 0,
		FIRST_FIT = 1,
		BEST_FIT = 2,
		OPTIMUM_Fit = 3,
	};
	extern AlignMode align_mode_;

	extern long space_width_;
	extern long stretchability_;
	extern long shrinkability_;

	// 1 inch = 25.4 mm
	inline extern float mm_to_inch(float a) { return a / 25.4; }

	// not so sure if I am doing this correctly
	inline extern float mm_to_fixed_point(float a) { return a * dpi_ * 64 / 25.4; }

	inline extern int display_width() { return page_width_ * dpi_ / 25.4; }
	inline extern int display_height() { return page_height_ * dpi_ / 25.4; }

	// space in char exclude all margins.
	inline extern int content_width_fixed_point() { return mm_to_fixed_point(page_width_ - margin_left_ - margin_right_); }
	inline extern int content_height_fixed_point() { return mm_to_fixed_point(page_height_ - margin_top_ - margin_bottom_); }

	extern float AdjustmentRatio(long L, long l);

	//borders
	extern bool border_[];
	extern std::string border_color[];

	extern bool show_river_;
};

