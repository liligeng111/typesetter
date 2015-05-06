#pragma once
#include <string>
#include <vector>
#include "box.h"

using namespace std;

class River
{
public:
	River(int page);
	~River();
	void AddBox(Box* box) { list_.push_back(box); }
	void Analyse();
	int size() const { return list_.size(); }
	int id() const { return id_; }
	int page() const { return page_; }

	void SVG(ofstream& file) const;
	static double ratio_;

	long local_deviation_;
	double global_deviation_;
	long width_sum_;
	double width_bar_;
	double width_std_;
	long min_left_length_;
	long min_right_length_;
	long up_length_, down_length_;
	double right_length_bar_;
	double left_length_bar_;
	double Lxx_, Lyy_, Lxy_;
	double alpha_;
	double beta_;
	double x_sum_, y_sum_;
	double xy_sum_, x_square_sum_, y_square_sum_;
	double x_bar_, y_bar_;
	double up_volume_, down_volume_, volume_;
	int repeats_;
	int repeat_words_;
	string repeat_word_;

private:
	vector<Box*> list_;
	//regression x = a + by
	int id_;
	int page_;

	static int next_id_;
	void Volume(long x, long y);
};

