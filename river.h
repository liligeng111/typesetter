#pragma once
#include <string>
#include <vector>
#include "box.h"

using namespace std;

class River
{
public:
	River();
	~River();
	void AddBox(Box* box) { list.push_back(box); }
	void Analyse();
	int size() const { return list.size(); }

	string SVG() const;

private:
	vector<Box*> list;
	long local_deviation_;
	long global_deviation_;

	//regression y = a + bx
	float alpha_;
	float beta_;
	long x_sum_;
	long y_sum_;
	float x_bar_;
	float y_bar_;

};

