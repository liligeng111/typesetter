#include "path.h"
#include "typesetter.h"
#include "fterrors.h"

int MoveTo(const FT_Vector* to, void* user)
{
	((string*)user)->append(" M " + to_string(to->x) + " " + to_string(to->y));
	return 0;
}

int LineTo(const FT_Vector* to, void* user)
{
	((string*)user)->append(" L " + to_string(to->x) + " " + to_string(to->y));
	return 0;
}

int ConicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
	((string*)user)->append(" Q " + to_string(control->x) + " " + to_string(control->y) + " " + to_string(to->x) + " " + to_string(to->y));
	return 0;
}

int CubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
	((string*)user)->append(" C " + to_string(control1->x) + " " + to_string(control1->y) + " " + to_string(control2->x) + " " + to_string(control2->y) + " " + to_string(to->x) + " " + to_string(to->y));
	return 0;
}


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

	FT_Outline_Funcs funcs;
	funcs.move_to = &MoveTo;
	funcs.line_to = &LineTo;
	funcs.conic_to = &ConicTo;
	funcs.cubic_to = &CubicTo;
	funcs.delta = 0;
	funcs.shift = 0;

	// Write the outline to SVG;
	svg_ = "";
	FT_Error error = FT_Outline_Decompose(&outline, &funcs, &svg_);
	if (error)
	{
		Typesetter::Message("Error decomposing outline: " + to_string(error));
	}
}


Path::~Path()
{
	delete points_;
	delete tags_;
	delete contours_;
}

