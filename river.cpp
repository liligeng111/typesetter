#include "river.h"
#include "settings.h"

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
	regression += "<path id='" + to_string(id_) + "' fill='none' stroke-width='0' stroke='rgb(25, 0, 250)' d='M " + to_string(alpha_ * settings::space_width_ + beta_ * list[0]->parent()->y()) + " " + to_string(list[0]->parent()->y());
	for (int i = 1; i < list.size(); i++)
	{
		str += " L " + to_string(list[i]->MidPoint().x()) + " " + to_string(list[i]->parent()->y());
		regression += " L " + to_string(alpha_ * settings::space_width_ + beta_ * list[i]->parent()->y()) + " " + to_string(list[i]->parent()->y());
	}
	str += "'/> \n";
	regression += "'/>";

	//str += "<path fill='none' stroke-width='64' stroke='rgb(25, 0, 250)' d='a b " + to_string(alpha_) + " " + to_string(beta_) + "'/>";
	//str += "<path fill='none' stroke-width='64' stroke='rgb(25, 0, 250)' d='lxx lyy lxy " + to_string(Lxx_) + " " + to_string(Lyy_) + " " + to_string(Lxy_) + "'/>";
	//str += "<path fill='none' stroke-width='64' stroke='rgb(25, 0, 250)' d='sum x y xy " + to_string(x_sum_) + " " + to_string(y_sum_) + " " + to_string(xy_sum_) + "'/>";
	//str += "<path fill='none' stroke-width='64' stroke='rgb(25, 0, 250)' d='sumsq x y " + to_string(x_square_sum_) + " " + to_string(y_square_sum_) + "'/>";
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
		float x = 1.0 * box->MidPoint().x() / settings::space_width_;
		float y = 1.0 * box->parent()->y() / settings::space_width_;
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