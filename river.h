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

	string SVG() const;

private:
	vector<Box*> list;
};

