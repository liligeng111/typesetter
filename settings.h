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

	extern std::string font_;
	extern int font_size_;

	extern long em_size_;
	extern long line_height_;
	extern long space_width_;
	extern long stretchability_;
	extern long shrinkability_;
	extern long descender_;

	// 1 inch = 25.4 mm
	inline extern float mm_to_inch(float a) { return a / 25.4; }

	inline extern float mm_to_point(float a) { return a * 72 * em_size_ / (25.4 * font_size_); }

	//use 96 as dpi
	inline extern int display_width() { return page_width_ * 96 / 25.4; }
	inline extern int display_height() { return page_height_ * 96 / 25.4; }

	// space in char exclude all margins.
	inline extern long content_width_point() { return mm_to_point(page_width_ - margin_left_ - margin_right_); }
	inline extern long content_height_point() { return mm_to_point(page_height_ - margin_top_ - margin_bottom_); }
	
	//borders
	extern bool border_[];
	extern int markdown_type_;

	extern bool show_river_;
	extern float river_threshold_;

	//algorithm related
	extern float rho_;
	extern float font_expansion_; //max of font expansion allowed
	extern float letter_space_stretch_ratio_;
	extern float letter_space_shrink_ratio_;
	extern float stretch_ratio_;
	extern float shrink_ratio_;
	extern int expansion_mode_;

	const int item_priority_size_ = 5; //number of possible priorities + 1
	const int end_paragraph_glue_priority_ = 1;
	const int box_priority_ = 2;
	const int letter_space_priority_ = 3;
	const int glue_priority_ = 4;

	//magic related
	extern bool use_magic_;
	extern float min_magic_gain_;
	extern float max_magic_amount_;
};

