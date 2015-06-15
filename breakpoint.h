#pragma once

#include "item.h"
#include <vector>

class Breakpoint
{
public:
	//TODO::merge
	struct Demerits
	{
		Demerits(float my_r, long my_length, int my_penalty, float my_result, long my_l)
		{
			r = my_r;
			length = my_length;
			penalty = my_penalty;
			result = my_result;
			l = my_l;
		}
		Demerits()
		{
			r = 0;
			length = 0;
			penalty = 0;
			result = 0;
			l = 0;
		}
		float r;
		long length;
		int penalty;
		float result;
		long l; //length with margin kerning;
	};

	Breakpoint(Item* item);
	Breakpoint(Item* item, int line, float demerits_sum, Demerits demerits, Breakpoint* prev);
	void init(Item* item, int line, float demerits_sum, Demerits demerits, Breakpoint* prev);
	~Breakpoint();

	Item* item() const { return item_; }
	int line() const { return line_; }
	Demerits demerits() const { return demerits_; }
	void set_demerits(Demerits d) { demerits_ = d; }
	float demerits_sum() const { return demerits_sum_; }
	void set_demerits_sum(float ds) { demerits_sum_ = ds; }
	Breakpoint* prev() const { return prev_; }
	void set_prev(Breakpoint* prev) { prev_ = prev; }
	vector<Breakpoint*>* next() { return &next_; }
	void push_next(Breakpoint* next) { next_.push_back(next); }
	float heuristic() const { return heuristic_; }
	void set_heuristic(float heuristic) { heuristic_ = heuristic; }

private:
	Item* item_;
	int line_; //# of line end at here
	Demerits demerits_;
	float demerits_sum_; //demerits up to this point, used by optimum fit
	Breakpoint* prev_;

	//for A* search
	vector<Breakpoint*> next_;
	float heuristic_;
};

