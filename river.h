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
	void AddBox(Box* box) { list.push_back(box); }
	void Analyse();
	int size() const { return list.size(); }
	int id() const { return id_; }
	int page() const { return page_; }
	long local_deviation() const { return local_deviation_; }
	float global_deviation() const { return global_deviation_; }

	void SVG(ofstream& file) const;
	static double ratio_;

private:
	vector<Box*> list;
	long local_deviation_;
	double global_deviation_;

	//regression x = a + by
	double alpha_;
	double beta_;
	double x_sum_, y_sum_;
	double xy_sum_, x_square_sum_, y_square_sum_;
	double x_bar_, y_bar_;
	int id_;
	int page_;

	static int next_id_;
	double Lxx_, Lyy_, Lxy_;
};

