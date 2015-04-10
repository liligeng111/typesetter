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
}


River::~River()
{
}


void River::SVG(ofstream& file) const
{
	file << "<path class='river' id='river" << id_ << "' local='" << local_deviation_ << "' global='" << global_deviation_ << "' d='M " << list[0]->MidPoint().x() << " " << list[0]->parent()->y() + settings::line_height_ / 2;
	for (int i = 1; i < list.size(); i++)
	{
		file << " L " << list[i]->MidPoint().x() << " " << list[i]->parent()->y() + settings::line_height_ / 2;
	}
	file << "'/> \n";

	file << "<path class='regression' id='regression" << id_ << "' d='M " << alpha_ * ratio_ + beta_ * list[0]->parent()->y() << " " << list[0]->parent()->y() + settings::line_height_ / 2;
	for (int i = 1; i < list.size(); i++)
	{
		file << " L " << alpha_ * ratio_ + beta_ * list[i]->parent()->y() << " " << list[i]->parent()->y() + settings::line_height_ / 2;
	}
	file << "'/> \n";

	/*
	file << "<path d='a b ratio" + to_string(alpha_) + " " + to_string(beta_) + " " + to_string(ratio_) + "'/>";
	file << "<path d='lxx lyy lxy " + to_string(Lxx_) + " " + to_string(Lyy_) + " " + to_string(Lxy_) + "'/>";
	file << "<path d='sum x y xy " + to_string(x_sum_) + " " + to_string(y_sum_) + " " + to_string(xy_sum_) + "'/>";
	file << "<path d='sumsq x y " + to_string(x_square_sum_) + " " + to_string(y_square_sum_) + "'/>";
	*/
}

void River::Analyse()
{
	//local first
	local_deviation_ = 0;
	for (int i = 1; i < list.size(); i++)
	{
		local_deviation_ += abs(list[i]->MidPoint().x() -  list[i - 1]->MidPoint().x());
	}
	local_deviation_ /= list.size() - 1;

	//then regression
	x_sum_ = 0;
	y_sum_ = 0;
	xy_sum_ = 0;
	x_square_sum_ = 0;
	y_square_sum_ = 0;
	for (Box* box : list)
	{
		double x = box->MidPoint().x() / ratio_;
		double y = box->parent()->y() / ratio_;
		x_sum_ += x;
		y_sum_ += y;
		xy_sum_ += x * y;
		x_square_sum_ += x * x;
		y_square_sum_ += y * y;
	}
	x_bar_ = x_sum_ / list.size();
	y_bar_ = y_sum_ / list.size();
	
	//float l_xy = 0;
	//float l_xx = 0;
	//for (Box* box : list)
	//{
	//	l_xy += (box->MidPoint().x() - x_bar_) * (box->parent()->y() - y_bar_);
	//	l_xx += (box->MidPoint().x() - x_bar_) * (box->MidPoint().x() - x_bar_);
	//}
	Lxx_ = x_square_sum_ - x_sum_ * x_sum_ / list.size();
	Lyy_ = y_square_sum_ - y_sum_ * y_sum_ / list.size();
	Lxy_ = xy_sum_ - x_sum_ * y_sum_ / list.size();
	beta_ = Lxy_ / Lyy_;
	alpha_ = x_bar_ - beta_ * y_bar_;

	if (Lxx_ == 0 || Lyy_ == 0)
		global_deviation_ = 1;
	else
		global_deviation_ = Lxy_ * Lxy_ / (Lxx_ * Lyy_);
	//if (beta_ == 0)
	//	return;

	//for (Box* box : list)
	//{
	//	global_deviation_ += ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x()) * ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x());
	//}
	//global_deviation_ /= list.size() - 2;
}