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
	long global_deviation() const { return global_deviation_; }

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
	int id_;
	int page_;

	static int next_id_;
};

