#include "river.h"

int River::next_id_ = 0;

River::River(int page)
{
	id_ = next_id_;
	next_id_++;
	page_ = page;
}


River::~River()
{
}


string River::SVG() const
{
	string str = "";	
	string regression = "";
	str += "<path id='" + to_string(id_) + "' fill='none' stroke-width='0' stroke='rgb(250, 0, 0)' d='M " + to_string(list[0]->MidPoint().x()) + " " + to_string(list[0]->parent()->y());
	regression += "<path id='" + to_string(id_) + "' fill='none' stroke-width='0' stroke='rgb(25, 0, 250)' d='M " + to_string(list[0]->MidPoint().x()) + " " + to_string(alpha_ + beta_ * list[0]->MidPoint().x());
	for (int i = 1; i < list.size(); i++)
	{
		str += " L " + to_string(list[i]->MidPoint().x()) + " " + to_string(list[i]->parent()->y());
		regression += " L " + to_string(list[i]->MidPoint().x()) + " " + to_string(alpha_ + beta_ * list[i]->MidPoint().x());
	}
	str += "'/>";
	regression += "'/>";

	//str += "<path fill='none' stroke-width='64' stroke='rgb(25, 0, 250)' d='M " + to_string(alpha_) + " " + to_string(beta_) + "'/>";
	return str + regression;
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
		x_sum_ += box->MidPoint().x();
		y_sum_ += box->parent()->y();
		xy_sum_ += box->MidPoint().x() * box->parent()->y();
		x_square_sum_ += box->MidPoint().x() * box->MidPoint().x();
		y_square_sum_ += box->parent()->y() * box->parent()->y();
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
	float Lxx = x_square_sum_ - x_sum_ * x_sum_ / list.size();
	float Lyy = y_square_sum_ - y_sum_ * y_sum_ / list.size();
	float Lxy = xy_sum_ - x_sum_ * y_sum_ / list.size();
	beta_ = Lxy / Lxx;
	alpha_ = y_bar_ - beta_ * x_bar_;

	if (abs(Lxx) < 0.0001 || abs(Lyy) < 0.0001)
		global_deviation_ = 1;
	else
		global_deviation_ = Lxy / sqrt(Lxx * Lyy);
	//if (beta_ == 0)
	//	return;

	//for (Box* box : list)
	//{
	//	global_deviation_ += ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x()) * ((box->parent()->y() - alpha_) / beta_ - box->MidPoint().x());
	//}
	//global_deviation_ /= list.size() - 2;
}