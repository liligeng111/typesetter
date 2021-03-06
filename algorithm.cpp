#include "Typesetter.h"
#include "viewer.h"
#include "settings.h"
#include <queue>
#include "logger.h"

void Typesetter::bidirection_magic()
{
	//erase pervious data
	//TODO::memory leak???
	for (auto bp : passive_list_)
	{
		//delete bp;
	}
	passive_list_.clear();
	local_cost_.clear();

	//line width 
	Item* item = new Item(Item::GLUE);
	item->init_glue(0, 0, settings::item_priority_size_ - 1);
	item->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	paragraph_.front()->set_prev(item);
	paragraph_.push_front(item);
	active_list_.push_back(new Breakpoint(paragraph_.back()));
	auto iter = paragraph_.rbegin();
	iter++;

	//set 0 demerits for begining and end
	paragraph_.front()->forward_demerits_ = 0;
	paragraph_.back()->backward_demerits_ = 0;

	while (iter != paragraph_.rend())
	{
		Item* b = *iter;
		iter++;

		Breakpoint* breakpoint = nullptr;
		if (!b->breakable())
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->penalty() > 999)
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
			Item* before = (*a)->item()->before();
			//TODO::Is this correct?
			float r;
			long L = (*a)->item()->before()->end_at() - b->after()->x();

			long l = settings::content_width_point();
			//margin kerning
			if (b->after()->glyph() != nullptr)
				l += b->after()->glyph()->left_protruding() * b->after()->glyph()->width() / 1000;

			if (after->break_glyph() != nullptr)
			{
				l += after->break_glyph()->right_protruding() * after->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			if (b->type() == Item::PENALITY)
			{
				forced_break = b->penalty() < -999;
			}

			L += after->break_width();

			int penalty = after->penalty(); //penalty
			if (L < l)
			{
				long Y = -(b->after()->sum_stretchability(0) - after->sum_stretchability(0));
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = -(b->after()->sum_shrinkability(0) - after->sum_shrinkability(0));
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
				
				Breakpoint::Demerits demerit(r, L, penalty, d, l);

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					(*a)->push_next(breakpoint);
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}
				if (b->backward_demerits_ > breakpoint->demerits_sum())
					b->backward_demerits_ = breakpoint->demerits_sum();
				local_cost_.insert(make_pair(make_pair((*a)->item(), b), demerit));
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

	active_list_.clear();
	//erase pervious data
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	passive_list_.clear();
	local_cost_.clear();

	//line width 
	long l;
	active_list_.push_back(new Breakpoint(paragraph_.front()));
	auto f_iter = paragraph_.begin();
	f_iter++;

	while (f_iter != paragraph_.end())
	{
		Item* b = *f_iter;
		//cout << b << " : " << b->content() << endl;
		f_iter++;
		//cout << b->content() << "  " << b->x() << " " << b->sum_y_ << " " << b->sum_z_ << endl;
		Breakpoint* breakpoint = nullptr;
		int penalty = b->penalty(); //penalty
		if (!b->breakable())
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->penalty() > 999)
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
			Item* before = b->before();
			float r;
			//TODO::precision issue?
			long L = before->end_at() - after->x();

			long l = settings::content_width_point();
			//margin kerning
			if (after->glyph() != nullptr)
				l += after->glyph()->left_protruding() * after->glyph()->width() / 1000;

			if (b->break_glyph() != nullptr)
			{
				l += b->break_glyph()->right_protruding() * b->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			L += b->break_width();
			if (b->type() == Item::PENALITY)
			{
				forced_break = penalty < -999;
			}
			if (L < l)
			{
				long Y = b->sum_stretchability(0) - after->sum_stretchability(0);
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_shrinkability(0) - after->sum_shrinkability(0);
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
				//if (r > 5)
				//	cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
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

				Breakpoint::Demerits demerit(r, L, penalty, d, l);

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					(*a)->push_next(breakpoint);
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}
				if (b->forward_demerits_ > breakpoint->demerits_sum())
					b->forward_demerits_ = breakpoint->demerits_sum();
				local_cost_.insert(make_pair(make_pair((*a)->item(), b), demerit));
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

	if (active_list_.size() != 1)
	{
		cout << "Content:: " << endl;
		for (Item* item : paragraph_)
		{
			cout << item->content() << endl;
		}
		cout << endl;
		cout << "active_list size: " << active_list_.size() << endl;
		message("ERROR: Unable to preform optimum fit");
		exit(EXIT_FAILURE);
	}	

	normal_demerits_ = active_list_.front()->demerits_sum();
	float max_importance = -1;
	//find magic edges from bp_a to bp_b
	//temporaryly put the root into passive list, triverse the list and put it back
	passive_list_.push_back(active_list_.front());

	auto bp_a = passive_list_.begin();
	while (bp_a != passive_list_.end())
	{
		auto bp_b = bp_a;
		bp_b++;
		while (bp_b != passive_list_.end())
		{
			float new_demerits = (*bp_a)->item()->forward_demerits_ + (*bp_b)->item()->backward_demerits_ + 1;
			float importance = normal_demerits_ - new_demerits;
			//check if is important
			if (importance < settings::min_magic_gain_)
			{
				bp_b++;
				continue;
			}

			if (importance > max_importance)
				max_importance = importance;
			//TODO::redunce, write a function to calculate l and L
			Item* after = (*bp_a)->item()->after();
			Item* before = (*bp_b)->item()->before();
			long L = before->end_at() - after->x();

			long l = settings::content_width_point();
			//margin kerning
			if (after->glyph() != nullptr)
				l += after->glyph()->left_protruding() * after->glyph()->width() / 1000;

			if ((*bp_b)->item()->break_glyph() != nullptr)
			{
				l += (*bp_b)->item()->break_glyph()->right_protruding() * (*bp_b)->item()->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			L += (*bp_b)->item()->break_width();
			if (abs(l - L) < settings::max_magic_amount_ * settings::em_size_)
			{
				//magic found! mark all item in between
				Item* end = (*bp_b)->item();
				Item* current = (*bp_a)->item();
				if (typesetting_single_paragraph_)
					magic_edges_.push_back(make_pair(current, end));
				while (!settings::use_magic_ && current != end)
				{
					current->set_is_magic(true);
					current = current->next();
					if (L < l && importance > current->magic_stretch_value_)
					{
						current->magic_stretch_value_ = importance;
					}

					if (L >= l && importance > current->magic_shrink_value_)
					{
						current->magic_shrink_value_ = importance;
					}
				}
			}
			else if (L > l)
			{
				//already too long, no need to search further
				break;
			}

			bp_b++;
		}

		bp_a++;
	}

	sort(magic_edges_.begin(), magic_edges_.end(), [](const pair<Item*, Item*> & a, const pair<Item*, Item*> & b)
	{
		return a.first->forward_demerits_ + a.second->backward_demerits_ > b.first->forward_demerits_ + b.second->backward_demerits_;
	});

	if (max_importance > 0)
		suggestions_.push_back(make_pair(paragraph_number_, max_importance));

	if (settings::use_magic_ && typesetting_single_paragraph_ && magic_edges_.size() > 0)
		optimum_fit_with_magic();
}


void Typesetter::optimum_fit_with_magic()
{
	active_list_.clear();
	//erase pervious data
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	passive_list_.clear();
	local_cost_.clear();

	//line width 
	long l;
	Item* item;
	active_list_.push_back(new Breakpoint(paragraph_.front()));
	auto f_iter = paragraph_.begin();
	f_iter++;
	bool found = false;

	while (f_iter != paragraph_.end())
	{
		Item* b = *f_iter;
		//cout << b << " : " << b->content() << endl;
		f_iter++;
		//cout << b->content() << "  " << b->x() << " " << b->sum_y_ << " " << b->sum_z_ << endl;
		Breakpoint* breakpoint = nullptr;
		int penalty = b->penalty(); //penalty
		if (!b->breakable())
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->penalty() > 999)
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
			Item* before = b->before();
			float r;
			//TODO::precision issue?
			long L = before->end_at() - after->x();

			long l = settings::content_width_point();
			//margin kerning
			if (after->glyph() != nullptr)
				l += after->glyph()->left_protruding() * after->glyph()->width() / 1000;

			if (b->break_glyph() != nullptr)
			{
				l += b->break_glyph()->right_protruding() * b->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			L += b->break_width();
			if (b->type() == Item::PENALITY)
			{
				forced_break = penalty < -999;
			}
			if (L < l)
			{
				long Y = b->sum_stretchability(0) - after->sum_stretchability(0);
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_shrinkability(0) - after->sum_shrinkability(0);
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
			//the magic edge currently using
			if (!found && magic_index_ >= 0 && magic_index_ < magic_edges_.size())
			{
				auto edge = magic_edges_[magic_index_];
				if (edge.first == (*a)->item() && edge.second == b)
				{
					r = 0;
					//TODO::should we do this?
					penalty = 0;
					found = true;
				}
			}

			if (r >= -1 && r < settings::rho_)
			{
				//if (r > 5)
				//	cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
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


				Breakpoint::Demerits demerit(r, L, penalty, d, l);

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					(*a)->push_next(breakpoint);
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}
			}

			if (forced_break)
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

	if (active_list_.size() != 1)
	{
		cout << "Content:: " << endl;
		for (Item* item : paragraph_)
		{
			cout << item->content() << endl;
		}
		cout << endl;
		cout << "active_list size: " << active_list_.size() << endl;
		message("ERROR: Unable to preform optimum fit");
		exit(EXIT_FAILURE);
	}

	if (!found)
	{
		Logger::error(300, "Cannot find the magic edge");
	}
}

void Typesetter::optimum_fit_magic_edge(vector<pair<Item*, Item*>> magic_found)
{
	//erase pervious data
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	passive_list_.clear();
	local_cost_.clear();

	//line width 
	long l;
	active_list_.push_back(new Breakpoint(paragraph_.front()));
	auto iter = paragraph_.begin();
	iter++;

	while (iter != paragraph_.end())
	{
		Item* b = *iter;
		//cout << b << " : " << b->content() << endl;
		iter++;
		//cout << b->content() << "  " << b->x() << " " << b->sum_y_ << " " << b->sum_z_ << endl;
		Breakpoint* breakpoint = nullptr;
		Breakpoint* magic_breakpoint = nullptr;
		int penalty = b->penalty(); //penalty
		if (!b->breakable())
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->penalty() > 999)
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
			Item* before = b->before();
			float r;
			//TODO::precision issue?
			long L = before->end_at() - after->x();

			long l = settings::content_width_point();
			//margin kerning
			if (after->glyph() != nullptr)
				l += after->glyph()->left_protruding() * after->glyph()->width() / 1000;

			if (b->break_glyph() != nullptr)
			{
				l += b->break_glyph()->right_protruding() * b->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			L += b->break_width();
			if (b->type() == Item::PENALITY)
			{
				forced_break = penalty < -999;
			}
			if (L < l)
			{
				long Y = b->sum_stretchability(0) - after->sum_stretchability(0);
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_shrinkability(0) - after->sum_shrinkability(0);
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

			//rho here
			if (r >= -1 && r < settings::rho_)
			{
				//if (r > 5)
				//	cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
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

				Breakpoint::Demerits demerit(r, L, penalty, d, l);

				if ((*a)->magic_count() == 0)
				{

					if (breakpoint == nullptr)
					{
						breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
						(*a)->push_next(breakpoint);
					}
					//better breakpoint
					else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
					{
						breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					}

					local_cost_.insert(make_pair(make_pair((*a)->item(), b), demerit));
				}



				//magic breakpoint
				if (magic_breakpoint == nullptr)
				{
					magic_breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					(*a)->push_next(magic_breakpoint);
					magic_breakpoint->set_magic_count((*a)->magic_count());
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < magic_breakpoint->demerits_sum())
				{
					magic_breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					magic_breakpoint->set_magic_count((*a)->magic_count());
				}

				
			}

			if ((*a)->magic_count() == 0 && abs(l - L) < settings::max_magic_amount_ * settings::em_size_ && (!forced_break))
			{
				//magic edge

				//if this magic if not found before
				bool new_magic = true;
				for (auto bp : magic_found)
				{
					if ((*a)->item() == bp.first && b == bp.second)
					{
						new_magic = false;
					}

					//Item* aa = (*a)->item();
					//cout << (*a)->item()->next() << " " << bp.first->next() << endl;
				}

				if (new_magic)
				{
					Breakpoint::Demerits demerit(r, L, penalty, 0, l);

					Breakpoint* magic_edge = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum(), demerit, (*a));
					magic_edge->set_is_magic(true);
					magic_edge->set_magic_count(1);

					active_list_.push_back(magic_edge);
				}
			}

			if ((r < -settings::max_magic_amount_ || forced_break))
			{
				//cout << "deletefrom active list: " << r << endl;
				//cout << "active_list size: " << active_list_.size() << endl;
				passive_list_.push_back(*a);
				a = active_list_.erase(a);
			}
			else
			{
				a++;
			}
		}

		//if (a == end)
		//{
		//	cout << "active_list size: " << active_list_.size() << endl;
		//}
		//TODO::what is q?
		if (breakpoint != nullptr)
		{
			active_list_.push_back(breakpoint);
		}
		if (magic_breakpoint != nullptr)
		{
			active_list_.push_back(magic_breakpoint);
		}

	}

	if (active_list_.size() != 2 && active_list_.size() != 1)
	{
		cout << "active_list size: " << active_list_.size() << endl;
		message("ERROR: Unable to preform optimum fit");
		exit(EXIT_FAILURE);
	}

}
void Typesetter::optimum_fit()
{
	//erase pervious data
	for (auto bp : passive_list_)
	{
		delete bp;
	}
	passive_list_.clear();
	local_cost_.clear();

	//line width 
	long l;
	Item* item = new Item(Item::GLUE);
	item->init_glue(0, 0, settings::item_priority_size_ - 1);
	item->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	paragraph_.front()->set_prev(item);
	paragraph_.push_front(item);
	active_list_.push_back(new Breakpoint(item));
	auto iter = paragraph_.begin();
	iter++;

	while (iter != paragraph_.end())
	{
		Item* b = *iter;
		//cout << b << " : " << b->content() << endl;
		iter++;
		//cout << b->content() << "  " << b->x() << " " << b->sum_y_ << " " << b->sum_z_ << endl;
		Breakpoint* breakpoint = nullptr;
		int penalty = b->penalty(); //penalty
		if (!b->breakable())
		{
			continue;
		}
		else if (b->type() == Item::PENALITY)
		{
			// must not break
			if (b->penalty() > 999)
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
			Item* before = b->before();
			float r;
			//TODO::precision issue?
			long L = before->end_at() - after->x();

			long l = settings::content_width_point();
			//margin kerning
			if (after->glyph() != nullptr)
				l += after->glyph()->left_protruding() * after->glyph()->width() / 1000;

			if (b->break_glyph() != nullptr)
			{
				l += b->break_glyph()->right_protruding() * b->break_glyph()->width() / 1000;
			}
			else if (before->glyph() != nullptr)
			{
				l += before->glyph()->right_protruding() * before->glyph()->width() / 1000;
			}

			L += b->break_width();
			if (b->type() == Item::PENALITY)
			{
				forced_break = penalty < -999;
			}
			if (L < l)
			{
				long Y = b->sum_stretchability(0) - after->sum_stretchability(0);
				if (Y > 0)
					r = 1.0f * (l - L) / Y;
				else
					r = 1000;
			}
			else if (L > l)
			{
				long Z = b->sum_shrinkability(0) - after->sum_shrinkability(0);
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
				//if (r > 5)
				//	cout << (*a)->item()->content() << " ----> " << b->content() << " L: " << L << " r: " << r << endl;
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

				Breakpoint::Demerits demerit(r, L, penalty, d, l);

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
					(*a)->push_next(breakpoint);
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}

				local_cost_.insert(make_pair(make_pair((*a)->item(), b), demerit));
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

	if (active_list_.size() != 1)
	{
		cout << "Content:: " << endl;
		for (Item* item : paragraph_)
		{
			cout << item->content() << endl;
		}
		cout << endl;
		cout << "active_list size: " << active_list_.size() << endl;
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
{/*
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
	box->sum_y_font_ = 0;
	box->sum_z_font_ = 0;
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
				demerit.length = L;
				demerit.result = d;

				if (breakpoint == nullptr)
				{
					breakpoint = new Breakpoint(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}
				//better breakpoint
				else if ((*a)->demerits_sum() + d < breakpoint->demerits_sum())
				{
					breakpoint->init(b, (*a)->line() + 1, (*a)->demerits_sum() + d, demerit, (*a));
				}
				Breakpoint::Demerits deme = { r, L, penalty, d };
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
	*/
}

void Typesetter::break_paragraph()
{
	//reverse_optimum_fit();

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
	//A_star();

	Item* item = new Item(Item::GLUE);
	item->init_glue(0, 0, settings::item_priority_size_ - 1);
	item->set_geometry(paragraph_.front()->x(), 0, 0, 0);
	paragraph_.front()->set_prev(item);
	paragraph_.push_front(item);
	//optimum_fit_magic_edge(magic_found);
	bidirection_magic();
	//insert breakpoints (for knuth original algorithm)

	Breakpoint* bp = active_list_.front();
	bp->set_is_last(true);

	auto pos = breakpoints.rbegin();
	while (bp->prev() != nullptr)
	{
		//cout << bp->item()->after()->content() << endl;
		//cout << "TOTAL: " << bp->demerits_sum() << "  " << bp->item() << "  " << bp->demerits().result << "  " << bp->demerits().l << " " << bp->demerits().length << "  " << bp->demerits().r << endl;
		//cout << bp->item()->forward_demerits_ << " " << bp->item()->backward_demerits_ << "  " << bp->item()->forward_demerits_ + bp->item()->backward_demerits_ << endl;
		breakpoints.insert(pos.base(), bp);
		bp = bp->prev();
		pos++;
	}
	
	while (!active_list_.empty())
	{
		passive_list_.push_back(active_list_.front());
		active_list_.pop_front();
	}

	items_.splice(items_.end(), paragraph_);
	paragraph_number_++;
}