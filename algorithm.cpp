#include "Typesetter.h"
#include "viewer.h"
#include "settings.h"
#include <queue>

void Typesetter::optimum_fit()
{
	//erase pervious data
	passive_list_.clear();
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	local_cost_.clear();

	//line width 
	long l = settings::content_width_point();
	Glue* box = new Glue(0, 0, 0);
	box->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	box->sum_y_ = 0;
	box->sum_z_ = 0;
	paragraph_.front()->set_prev(box);
	paragraph_.push_front(box);
	active_list_.push_back(new Breakpoint(box));
	auto iter = paragraph_.begin();
	iter++;

	while (iter != paragraph_.end())
	{
		Item* b = *iter;
		//cout << b << " : " << b->content() << endl;
		iter++;
		//if (b->x() != b->sum_w_)
		//	cout << b->content() << "  " << b->x() << " " << b->sum_w_ << endl;
		Breakpoint* breakpoint = nullptr;
		int penalty = b->p(); //penalty
		if (b->type() == Item::BOX)
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->p() > 999)
			{
				continue;
			}
		}

		//potential break
		auto a = active_list_.begin();
		auto end = active_list_.end();

		//iterare the active list
		while (a != end)
		{
			//calculate r
			bool forced_break = false;
			Item* after = (*a)->item()->after();
			float r;
			long L = b->x() - after->x();
			if (b->type() == Item::PENALITY)
			{
				L += b->width();
				forced_break = penalty < -999;
			}
			if (L < l)
			{
				long Y = b->sum_y_ - after->sum_y_;
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_z_ - after->sum_z_;
				if (Z > 0)
					r = 1.0f * (l - L) / Z;
				else
					r = 1000;
			}
			else
			{
				//prefect!
				r = 0;
			}

			//if (L == 0)
			//{
			//	for (Item* item : paragraph_)
			//	{
			//		//cout << item << " : " << item->content() << endl;
			//	}
			//	cout << after->content() << " ----> " << b->content() << " L: " << L << " r: " << r << " a: " << after->sum_y_ << " b: " << b->sum_y_ << endl;
			//	cout << (*a)->item()->content() << " ----> " << b->content() << endl << endl;
			//}
			//cout << "r " << r << endl;
			//rho here
			if (r >= -1 && r < settings::rho_)
			{
				//cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
				float d;
				if (penalty >= 0)
				{
					d = 1 + 100 * r * r * abs(r) + penalty;
					d = d * d;
				}
				else if (penalty > -1000)
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d - penalty * penalty;
				}
				else
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d;
				}
				//TODO::f(a) and c
				//new breakpoint

				Breakpoint::Demerits demerit;
				demerit.penalty = penalty;
				demerit.r = r;
				demerit.result = d;

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
				Breakpoint::Demerits deme = { r, penalty, d };
				local_cost_.insert(make_pair(make_pair((*a)->item(), b), deme));
				(*a)->push_next(breakpoint);
			}

			if (r < -1 || forced_break)
			{
				//cout << "deletefrom active list: " << *a << endl;
				passive_list_.push_back(*a);
				a = active_list_.erase(a);
			}
			else
			{
				a++;
			}
		}
		//TODO::what is q?
		if (breakpoint != nullptr)
		{
			active_list_.push_back(breakpoint);
		}

	}
	if (active_list_.empty())
	{
		message("ERROR: Unable to preform optimum fit");
		exit(EXIT_FAILURE);
	}

}

void Typesetter::A_star()
{
	int max_size = 350;
	struct Node
	{
		list<Breakpoint*>* path;
		float demerits;
		//TODO::memory leak
		//~Node() { delete path; }
		bool operator<(const Node& node) const
		{
			return demerits + path->back()->heuristic() > node.demerits + node.path->back()->heuristic();
		}
	};

	priority_queue<Node> queue;
	queue.push({ new list<Breakpoint*>(1, active_list_.front()), 0 });
	Node node = queue.top();
	while (node.path->back() != passive_list_.back())
	{
		//cout << "Path: ";
		//for (Breakpoint* bp : *(node.path))
		//{
		//	cout << bp->item()->after()->word_content() << " ";
		//}
		//cout << " || cost: " << node.demerits + node.path->back()->heuristic();
		//cout << " L: " << node.path->back()->L << " a: " << node.path->back()->a << " b: " << node.path->back()->b << endl;

		queue.pop();
		Breakpoint* last = node.path->back();
		//add all possible nodes
		auto end = last->next()->end();
		for (auto next = last->next()->begin(); next != end; next++)
		{
			auto path = new list<Breakpoint*>(node.path->begin(), node.path->end());
			path->push_back(*next);
			//TODO::function for demerits
			float demerites = node.demerits + local_cost_[make_pair(last->item(), (*next)->item())].result;
			if ((*next)->item()->after()->word_content() == last->item()->after()->word_content())
			{
				//cout << (*next)->item()->after()->word_content() << " " << last->item()->after()->word_content() << endl;
				demerites += 2500;
			}
			auto iter = node.path->rbegin();
			iter++;
			if (iter != node.path->rend())
			{
				float r1 = local_cost_[make_pair(last->item(), (*next)->item())].r;
				float r2 = local_cost_[make_pair((*iter)->item(), last->item())].r;
				float dr = (r1 - r2) / 2;
				dr = dr * dr;
				demerites += 10000 * dr * dr * dr;
				//cout << (*next)->item()->after()->word_content() << "  " << last->item()->after()->word_content() << "  " << endl;
				//cout << r1 << "  " << r2 << "  " << 1000 * dr << "  " << demerites << endl << endl;
			}
			queue.push({ path, demerites });
			if (queue.size() > max_size)
				max_size = queue.size();
		}

		delete node.path;
		node = queue.top();
		//if (max_size > 350)
		//	cout << "Max queue size: " << max_size << endl;
	}

	//insert breakpoints (for A*)
	auto bp = node.path->begin();
	Breakpoint* prev = node.path->front();
	for (bp++; bp != node.path->end(); bp++)
	{
		//cout << bp->item()->after()->content() << endl;
		breakpoints.push_back(*bp);
		(*bp)->set_demerits(local_cost_[make_pair(prev->item(), (*bp)->item())]);
		prev = *bp;
	}

	//release memory
	while (!queue.empty())
	{
		delete queue.top().path;
		queue.pop();
	}
}

void Typesetter::reverse_optimum_fit()
{
	//erase pervious data
	passive_list_.clear();
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	local_cost_.clear();

	//line width 
	long l = settings::content_width_point();
	Glue* box = new Glue(0, 0, 0);
	box->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	box->sum_y_ = 0;
	box->sum_z_ = 0;
	paragraph_.front()->set_prev(box);
	paragraph_.push_front(box);
	active_list_.push_back(new Breakpoint(paragraph_.back()));
	auto iter = paragraph_.rbegin();
	iter++;

	while (iter != paragraph_.rend())
	{
		Item* b = *iter;
		//cout << b << " : " << b->content() << endl;
		iter++;

		//if (b->x() != b->sum_w_)
		//	cout << b->content() << "  " << b->x() << " " << b->sum_w_ << endl;
		Breakpoint* breakpoint = nullptr;
		if (b->type() == Item::BOX)
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->p() > 999)
			{
				continue;
			}
		}

		//potential break
		auto a = active_list_.begin();
		auto end = active_list_.end();

		//iterare the active list
		while (a != end)
		{
			//calculate r
			//force break for first one
			bool forced_break = (iter == paragraph_.rend());
			Item* after = (*a)->item();
			float r;
			long L = -(b->after()->x() - after->x());
			if (b->type() == Item::PENALITY)
			{
				forced_break = b->p() < -999;
			}
			if (after->type() == Item::PENALITY)
			{
				L += after->width();
			}
			int penalty = after->p(); //penalty
			if (L < l)
			{
				long Y = -(b->after()->sum_y_ - after->sum_y_);
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = -(b->after()->sum_z_ - after->sum_z_);
				if (Z > 0)
					r = 1.0f * (l - L) / Z;
				else
					r = 1000;
			}
			else
			{
				//prefect!
				r = 0;
			}
			//cout << after->content() << " ----> " << b->content() << " L: " << L << " r: " << r << " a: " << after->sum_y_ << " b: " << b->sum_y_ << endl;
			//cout << (*a)->item()->content() << " ----> " << b->content() << endl << endl;
			//cout << "r " << r << endl;
			//rho here
			if (r >= -1 && r < settings::rho_)
			{
				//TODO::cout result seems weird
				//cout << (*a)->item()->after()->word_content() << " ----> " << b->after()->word_content() << " L: " << L << " r: " << r << endl;

				float d;
				if (penalty >= 0)
				{
					d = 1 + 100 * r * r * abs(r) + penalty;
					d = d * d;
				}
				else if (penalty > -1000)
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d - penalty * penalty;
				}
				else
				{
					d = 1 + 100 * r * r * abs(r);
					d = d * d;
				}
				//TODO::f(a) and c
				//new breakpoint

				Breakpoint::Demerits demerit;
				demerit.penalty = penalty;
				demerit.r = r;
				demerit.result = d;

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					breakpoint->L = L;
					breakpoint->a = after->x();
					breakpoint->b = b->x();
				}
				Breakpoint::Demerits deme = { r, penalty, d };
				local_cost_.insert(make_pair(make_pair(b, (*a)->item()), deme));
				breakpoint->push_next((*a));
			}

			if (r < -1 || forced_break)
			{
				//cout << "deletefrom active list: " << *a << endl;
				passive_list_.push_front(*a);
				a = active_list_.erase(a);
			}
			else
			{
				a++;
			}
		}
		//TODO::what is q?
		if (breakpoint != nullptr)
		{
			active_list_.push_back(breakpoint);
		}

	}
	if (active_list_.size() != 1)
	{
		cout << "Content:: " << endl;
		for (Item* item : paragraph_)
		{
			cout << item->content() << endl;
		}
		cout << endl;
		message("ERROR: Unable to preform optimum fit");
		exit(EXIT_FAILURE);
	}

	//cout << "First: " << active_list_.front()->item()->after()->word_content() << endl;
	//cout << "Size: " << active_list_.size() << endl;
}

void Typesetter::break_paragraph()
{
	reverse_optimum_fit();

	//find heuristic
	//TODO::test and delete pointers, local cost, push_next and stuff
	//for (auto breakpoint = passive_list_.rbegin(); breakpoint != passive_list_.rend(); breakpoint++)
	//{
	//	float min_heuristic = 10000000; //very large
	//	auto end = (*breakpoint)->next()->end();
	//	for (auto next = (*breakpoint)->next()->begin(); next != end; next++)
	//	{
	//		float heuristic = (*next)->heuristic() + local_cost_[make_pair((*breakpoint)->item(), (*next)->item())].result;
	//		min_heuristic = min(min_heuristic, heuristic);
	//	}
	//	(*breakpoint)->set_heuristic(min_heuristic);
	//}

	//for (auto breakpoint = passive_list_.begin(); breakpoint != passive_list_.end(); breakpoint++)
	//{
	//	Item* item = (*breakpoint)->item();
	//	cout << item->content() << " " << item->after()->content() << item->heuristic() << endl;
	//}
	A_star();

	//optimum_fit();
	////insert breakpoints (for knuth original algorithm)
	//Breakpoint* bp = active_list_.front();
	//auto pos = breakpoints.rbegin();
	//while (bp->prev() != nullptr)
	//{
	//	//cout << bp->item()->after()->content() << endl;
	//	breakpoints.insert(pos.base(), bp);
	//	bp = bp->prev();
	//	pos++;
	//}


	passive_list_.push_back(active_list_.front());
	active_list_.pop_front();
	//merge paragraph
	items_.splice(items_.end(), paragraph_);
}