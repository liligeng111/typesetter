#include "river.h"
#include "settings.h"
#include <fstream>

int River::next_id_ = 0;
double River::ratio_ = 64.0;

River::River(int page)
{
	id_ = next_id_;
	next_id_++;
	page_ = page;


	local_deviation_ = 0;
	x_sum_ = 0;
	y_sum_ = 0;
	xy_sum_ = 0;
	x_square_sum_ = 0;
	y_square_sum_ = 0;
	width_sum_ = 0;
	width_std_ = 0;
	min_left_length_ = 2147483647;
	min_right_length_ = 2147483647;
	right_length_bar_ = 0;
	left_length_bar_ = 0;
	up_length_ = 0;
	down_length_ = 0;
	up_volume_ = 99999;
	down_volume_ = -99999;
	repeats_ = 0;
	repeat_words_ = 0;
	repeat_word_ = "";
}


River::~River()
{
}


void River::SVG(ofstream& file) const
{
	/*
	file << "<path fill='none' class='river' id='river" << id_ << "' local='" << local_deviation_ << "' global='" << global_deviation_;
	file << "' width='" << volume_;
	file << "' d='M " << list_[0]->mid_point().x() << " " << list_[0]->parent()->y() + settings::line_height_ / 2;
	for (int i = 1; i < list_.size(); i++)
	{
		file << " L " << list_[i]->mid_point().x() << " " << list_[i]->parent()->y() + settings::line_height_ / 2;
	}
	file << "'/> \n";

	file << "<path fill='none' class='regression' id='regression" << id_;
	file << "' width='" << volume_;
	file << "' d='M " << alpha_ * ratio_ + beta_ * list_[0]->parent()->y() << " " << list_[0]->parent()->y() + settings::line_height_ / 2;
	for (int i = 1; i < list_.size(); i++)
	{
		file << " L " << alpha_ * ratio_ + beta_ * list_[i]->parent()->y() << " " << list_[i]->parent()->y() + settings::line_height_ / 2;
	}
	file << "'/> \n";

	/*
	file << "<path d='a b ratio" + to_string(alpha_) + " " + to_string(beta_) + " " + to_string(ratio_) + "'/>";
	file << "<path d='lxx lyy lxy " + to_string(Lxx_) + " " + to_string(Lyy_) + " " + to_string(Lxy_) + "'/>";
	file << "<path d='sum x y xy " + to_string(x_sum_) + " " + to_string(y_sum_) + " " + to_string(xy_sum_) + "'/>";
	file << "<path d='sumsq x y " + to_string(x_square_sum_) + " " + to_string(y_square_sum_) + "'/>";
	*/	
}

void River::Volume(long x, long y)
{
	double d = alpha_ * ratio_ + beta_ * y - x;
	if (d > 0)
	{
		if (d < up_volume_)
			up_volume_ = d;
	}
	else
	{
		if (d > down_volume_)
			down_volume_ = d;
	}
}

void River::Analyse()
{
	/*
	//local first
	for (int i = 1; i < list_.size(); i++)
	{
		local_deviation_ += abs(list_[i]->MidPoint().x() -  list_[i - 1]->MidPoint().x());
	}
	local_deviation_ /= list_.size() - 1;

	//then regression
	for (Box* box : list_)
	{
		double x = box->MidPoint().x() / ratio_;
		double y = box->parent()->y() / ratio_;
		x_sum_ += x;
		y_sum_ += y;
		xy_sum_ += x * y;
		x_square_sum_ += x * x;
		y_square_sum_ += y * y;
		width_sum_ += box->width();

		if (box->left() != NULL)
		{
			long length = box->left()->width();
			left_length_bar_ += length;
			if (length < min_left_length_)
				min_left_length_ = length;
		}

		if (box->right() != NULL)
		{
			long length = box->right()->width();
			right_length_bar_ += length;
			if (length < min_right_length_)
				min_right_length_ = length;
		}
	}
	x_bar_ = x_sum_ / list_.size();
	y_bar_ = y_sum_ / list_.size();
	width_bar_ = width_sum_ / list_.size();
	left_length_bar_ /= list_.size();
	right_length_bar_ /= list_.size();

	for (Box* box : list_)
	{
		width_std_ += (box->width() - width_bar_) * (box->width() - width_bar_) / list_.size();
	}
	width_std_ = sqrt(width_std_);
	
	//float l_xy = 0;
	//float l_xx = 0;
	//for (Box* box : list_)
	//{
	//	l_xy += (box->MidPoint().x() - x_bar_) * (box->parent()->y() - y_bar_);
	//	l_xx += (box->MidPoint().x() - x_bar_) * (box->MidPoint().x() - x_bar_);
	//}
	Lxx_ = x_square_sum_ - x_sum_ * x_sum_ / list_.size();
	Lyy_ = y_square_sum_ - y_sum_ * y_sum_ / list_.size();
	Lxy_ = xy_sum_ - x_sum_ * y_sum_ / list_.size();
	beta_ = Lxy_ / Lyy_;
	alpha_ = x_bar_ - beta_ * y_bar_;

	if (Lxx_ == 0 || Lyy_ == 0)
		global_deviation_ = 1;
	else
		global_deviation_ = Lxy_ * Lxy_ / (Lxx_ * Lyy_);
	//if (beta_ == 0)
	//	return;

	//for (Box* box : list_)
	//{
	//	global_deviation_ += ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x()) * ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x());
	//}
	//global_deviation_ /= list_.size() - 2;

	//up
	//const Box* up = list_[0]->parent()->left();
	//if (up == NULL || up->EndAt() < list_[0]->EndAt())
	//{
	//	up_length_ = 1;
	//}
	//else
	//{
	//	const vector<Box*>* line = up->children();
	//	for (int i = line->size() - 1; i >= 0; i--)
	//	{
	//		if (line->at(i)->type() != Box::BoxType::SPACE)
	//			continue;
	//		if ((list_[0]->x() - line->at(i)->EndAt()) * (list_[0]->EndAt() - line->at(i)->x()) < 0)
	//		{
	//			up_length_ = 1;
	//			break;
	//		}
	//	}
	//}

	//down
	//const Box* down = list_[list_.size() - 1]->parent()->right();
	//if (down == NULL || down->EndAt() < list_[list_.size() - 1]->EndAt())
	//{
	//	down_length_ = 1;
	//}
	//else
	//{
	//	const vector<Box*>* line = down->children();
	//	for (int i = line->size() - 1; i >= 0; i--)
	//	{
	//		if (line->at(i)->type() != Box::BoxType::SPACE)
	//			continue;
	//		if ((list_[list_.size() - 1]->x() - line->at(i)->EndAt()) * (list_[list_.size() - 1]->EndAt() - line->at(i)->x()) < 0)
	//		{
	//			down_length_ = 1;
	//			break;
	//		}
	//	}
	//}

	for (int i = 0; i < list_.size(); i++)
	{
		Box* box = list_[i];
		const Box* l = box->left()->MYWORD;
		if (l != NULL)
		{
			Volume(l->EndAt(), box->left()->parent()->y());
			Volume(l->EndAt(), box->left()->parent()->y() + l->height());

			if (i != 0)
			{
				if (l->Last()->glyph()->content() == list_[i - 1]->left()->MYWORD->Last()->glyph()->content())
					repeats_++;
				if (box->parent()->justify() && l->content() == list_[i - 1]->left()->MYWORD->content())
				{
					repeat_words_++;
					repeat_word_ = list_[i - 1]->left()->content();
				}
			}
		}

		const Box* r = box->right()->MYWORD;
		if (r != NULL)
		{
			Volume(r->x(), box->right()->parent()->y());
			Volume(r->x(), box->right()->parent()->y() + r->height());

			if (i != 0)
			{
				if (r->First()->glyph()->content() == list_[i - 1]->right()->MYWORD->First()->glyph()->content())
					repeats_++;
				if (box->parent()->justify() && r->content() == list_[i - 1]->right()->MYWORD->content())
				{
					repeat_words_++;
					repeat_word_ = list_[i - 1]->left()->content();
				}
			}
		}
	}
	volume_ = (up_volume_ - down_volume_) / sqrt(1 + beta_ * beta_);
	*/
}