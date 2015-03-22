#include "path.h"

Path::Path(FT_Outline outline)
{
	n_contours_ = outline.n_contours;
	n_points_ = outline.n_points;
	points_ = new Vector3l[n_points_];
	tags_ = new char[n_points_];
	contours_ = new int[n_contours_];
	flags_ = outline.flags;

	for (int i = 0; i < n_points_; i++)
	{
		points_[i] = Vector3l(outline.points[i].x, outline.points[i].y);
		tags_[i] = outline.tags[i];
	}

	for (int i = 0; i < n_contours_; i++)
	{
		contours_[i] = outline.contours[i];
	}
}


Path::~Path()
{
	delete points_;
	delete tags_;
	delete contours_;
}
