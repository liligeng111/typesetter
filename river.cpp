#include "river.h"


River::River()
{
}


River::~River()
{
}


string River::SVG() const
{
	string str = "";
	str += "<path fill='none' stroke-width='64' stroke='rgb(250, 0, 0)' d='M " + to_string(list[1]->MidPoint().x()) + " " + to_string(list[1]->parent()->MidPoint().y()) + " L " + to_string(list[0]->MidPoint().x()) + " " + to_string(list[0]->parent()->MidPoint().y()) + " " + to_string(list[2]->MidPoint().x()) + " " + to_string(list[2]->parent()->MidPoint().y()) + "'/>";
	return str;
}