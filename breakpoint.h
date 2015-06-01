#pragma once

#include "item.h"
#include <vector>

class Breakpoint
{
public:
	//TODO::merge
	struct Demerit
	{
		float r;
		int penalty;
		float result;
	};

	Breakpoint(Item* item);
	Breakpoint(Item* item, int line, float demerits, float r, Breakpoint* prev);
	void init(Item* item, int line, float demerits, float r, Breakpoint* prev);
	~Breakpoint();

	Item* item() const { return item_; }
	int line() const { return line_; }
	float demerits() const { return demerits_; }
	float r() const { return r_; }
	void set_r(float r) { r_ = r; }
	Breakpoint* prev() const { return prev_; }
	void set_prev(Breakpoint* prev) { prev_ = prev; }
	vector<Breakpoint*>* next() { return &next_; }
	void push_next(Breakpoint* next) { next_.push_back(next); }
	float heuristic() const { return heuristic_; }
	void set_heuristic(float heuristic) { heuristic_ = heuristic; }

	long L, a, b; //for debug
private:
	Item* item_;
	int line_; //# of line end at here
	float demerits_;
	float r_;
	Breakpoint* prev_;

	//for A* search
	vector<Breakpoint*> next_;
	float heuristic_;
};

