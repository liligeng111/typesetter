#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	parent_ = parent;
	page_ = nullptr;

	path_base_ = 0;
	font_base_ = 0;
	path_count_ = 0;
	width = 0;
	height = 0;

	GLboolean hasDSA;

	QSurfaceFormat format;
	//TODO::what version should I use?
	//format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setSamples(8);
	setFormat(format);

	initModelAndViewMatrices();
}

GLWidget::~GLWidget()
{
	cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}

void GLWidget::initModelAndViewMatrices()
{
	float tmp[2][3];
	translate(tmp, settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_));
	scale(model, 3.5 / settings::mm_to_point(1), 3.5 / settings::mm_to_point(1));
	mul(model, model, tmp);
	translate(view, 0, 0);
}

void GLWidget::cleanup()
{
	makeCurrent();
	doneCurrent();
}

void GLWidget::render_page(Page* page)
{
	page_ = page;
	//delete previous page
	if (path_count_ != 0)
		glDeletePathsNV(path_base_, path_count_);
	path_count_ = 0;
	path_base_ = 0;
	initGraphics();
	update();
}

void GLWidget::initGraphics()
{
	glMatrixLoadIdentityEXT(GL_PROJECTION);
	glMatrixOrthoEXT(GL_PROJECTION, 0, width, height, 0, -1, 1);
	if (page_ == nullptr)
	{
		return;
	}

	const int numChars = 256; // ISO/IEC 8859-1 8-bit character range
	GLuint templatePathObject = ~0; // Non-existant path object
	const GLfloat emScale = 1024; // match TrueType convention
	font_base_ = glGenPathsNV(numChars);
	glPathGlyphRangeNV(font_base_, GL_SYSTEM_FONT_NAME_NV, "Helvetica", GL_BOLD_BIT_NV, 0, numChars, GL_SKIP_MISSING_GLYPH_NV, templatePathObject, emScale);

	vector<const char*> paths;
	line_num_.clear();
	char_num_.clear();
	for (int l = 0; l < page_->children_size(); l++)
	{
		Line* line = static_cast<Line*>(page_->child(l));
		for (int c = 0; c < line->children_size(); c++)
		{
			if (line->child(c)->glyph() != nullptr)
			{
				const char* svg_str = line->child(c)->glyph()->path()->c_str();
				paths.push_back(svg_str);
				line_num_.push_back(l);
				char_num_.push_back(c);
			}
			else if (line->child(c)->is_break() && line->child(c)->break_glyph() != nullptr)
			{
				const char* svg_str = line->child(c)->break_glyph()->path()->c_str();
				paths.push_back(svg_str);
				line_num_.push_back(l);
				char_num_.push_back(c);
			}
		}
	}

	unsigned int i;
	path_count_ = paths.size();
	//extra one for make the paper white
	path_base_ = glGenPathsNV(path_count_ + 1);

	for (i = 0; i < paths.size(); i++)
	{
		const char *svg_str = paths[i];
		size_t svg_len = strlen(paths[i]);
		GLfloat stroke_width = 1;

		glPathStringNV(path_base_ + i, GL_PATH_FORMAT_SVG_NV, (GLsizei)svg_len, svg_str);
		glPathParameterfNV(path_base_ + i, GL_PATH_STROKE_WIDTH_NV, stroke_width);
	}

	string border = "M 0 0 L 0 " + to_string(settings::content_height_point()) + " L " + to_string(settings::content_width_point()) + " " + to_string(settings::content_height_point()) + " L " + to_string(settings::content_width_point()) + " 0 L 0 0";
	const char* svg_str = border.c_str();
	glPathStringNV(path_base_ + i, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(svg_str), svg_str);
	glPathParameterfNV(path_base_ + i, GL_PATH_STROKE_WIDTH_NV, 1);
}

void GLWidget::initializeGL()
{
	context_ = context();
	initializeOpenGLFunctions();
	extension_loader::load_extension(context_);
}

void GLWidget::paintGL()
{

	glEnable(GL_STENCIL_TEST);

	glClearStencil(0);
	glClearColor(0.9, 0.9, 0.9, 0);
	glStencilMask(~0);
	glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//nothing to draw
	if (page_ == nullptr)
	{
		return;
	}

	glMatrixPushEXT(GL_MODELVIEW);
	{
		//borders
		{
			Transform3x2 mat;
			mul(mat, view, model);

			MatrixLoadToGL(mat);

			glColor3f(1, 1, 1);
			//GLuint stroke_color = 0x000000;
			//GLfloat stroke_width = 1;

			glStencilFillPathNV(path_base_ + path_count_, GL_COUNT_UP_NV, 0x1F);
			glCoverFillPathNV(path_base_ + path_count_, GL_BOUNDING_BOX_NV);
		}

		for (int i = 0; i < path_count_; i++)
		{
			Transform3x2 mat;
			mul(mat, view, model);

			Transform3x2 result;

			//TODO::what happened?
			if (line_num_[i] >= page_->children_size())
				continue;
			Line* line = static_cast<Line*>(page_->child(line_num_[i]));
			Transform3x2 line_trans;
			translate(line_trans, line->x(), line->y());
			Transform3x2 temp;
			scale(temp, 1, -1);
			mul(line_trans, line_trans, temp);
			mul(result, mat, line_trans);

			if (char_num_[i] >= line->children_size())
				continue;
			Item* item = line->child(char_num_[i]);
			Transform3x2 char_trans;
			translate(char_trans, item->x(), item->y());
			mul(result, result, char_trans);
			scale(char_trans, item->expansion(), 1);
			mul(result, result, char_trans);

			if (item->glyph() != nullptr)
			{
				translate(char_trans, -item->glyph()->hori_bearing_x(), -item->glyph()->hori_bearing_y());
				mul(result, result, char_trans);
			}
			else if (item->is_break() && item->break_glyph() != nullptr)
			{
				translate(char_trans, item->width() - item->break_glyph()->hori_bearing_x(), 0);
				mul(result, result, char_trans);
			}

			MatrixLoadToGL(result);

			if (item->is_magic())
			{
				//int sum = item->shrink_count_ + item->stretch_count_;
				float shrink_alpha = item->magic_shrink_value_ / (3 * settings::min_magic_gain_) ;
				float stretch_alpha = item->magic_stretch_value_ / (3 * settings::min_magic_gain_);

				//show only one color
				if (settings::show_only_one_suggestion_)
				{
					if (shrink_alpha > stretch_alpha)
					{
						stretch_alpha = 0;
					}
					else
					{
						shrink_alpha = 0;
					}
				}

				float r = (shrink_alpha * settings::shrink_color_.red() + stretch_alpha * settings::stretch_color_.red()) / 255;
				float g = (shrink_alpha * settings::shrink_color_.green() + stretch_alpha * settings::stretch_color_.green()) / 255;
				float b = (shrink_alpha * settings::shrink_color_.blue() + stretch_alpha * settings::stretch_color_.blue()) / 255;
				//cout << r << " " << g << " " << b << endl;
				float max = 1;
				if (r > max)
					max = r;
				if (g > max)
					max = g;
				if (b > max)
					max = b;

				r /= max;
				g /= max;
				b /= max;
				glColor3f(r, g, b);
			}
			else
			{
				glColor3f(0, 0, 0);
			}


			//GLuint stroke_color = 0x000000;
			//GLfloat stroke_width = 1;
			Transform3x2 cleartype;

			glColorMask(true, false, false, true);
			translate(temp, 1.0f / 3, 0);
			mul(cleartype, temp, result);
			MatrixLoadToGL(cleartype);
			glStencilFillPathNV(path_base_ + i, GL_COUNT_UP_NV, 0x1F);
			glCoverFillPathNV(path_base_ + i, GL_BOUNDING_BOX_NV);

			glColorMask(false, false, true, true);
			translate(temp, -1.0f / 3, 0);
			mul(cleartype, temp, result);
			MatrixLoadToGL(cleartype);
			glStencilFillPathNV(path_base_ + i, GL_COUNT_UP_NV, 0x3f);
			glCoverFillPathNV(path_base_ + i, GL_BOUNDING_BOX_NV);

			glColorMask(false, true, false, true);
			MatrixLoadToGL(result);
			glStencilFillPathNV(path_base_ + i, GL_COUNT_UP_NV, 0x3f);
			glCoverFillPathNV(path_base_ + i, GL_BOUNDING_BOX_NV);

			glColorMask(true, true, true, true);
		}

		//markdowns
		for (int i = 0; i < page_->children_size(); i++)
		{

			Transform3x2 mat;
			mul(mat, view, model);

			Transform3x2 result;

			if (i >= page_->children_size())
				continue;
			Line* line = static_cast<Line*>(page_->child(i));
			Transform3x2 line_trans;
			translate(line_trans, line->x(), line->y());
			Transform3x2 temp;
			scale(temp, 1, -1);
			mul(line_trans, line_trans, temp);
			mul(mat, mat, line_trans);

			//why bother the kernings
			GLfloat kerning[20];
			for (int j = 0; j < 20; j++)
				kerning[j] = 512 * j;

			//left
			string left_text = to_string(line->line_number());
			const char * left_str = left_text.c_str();

			translate(temp, -1.25 * settings::em_size_, 0);
			mul(result, mat, temp);
			MatrixLoadToGL(result);
			glColor3f(0, 0.2, 0.6);
			glStencilFillPathInstancedNV(left_text.length(), GL_UNSIGNED_BYTE, left_str, font_base_, GL_PATH_FILL_MODE_NV, 0xFF, GL_TRANSLATE_X_NV, kerning);
			glCoverFillPathInstancedNV(left_text.length(), GL_UNSIGNED_BYTE, left_str, font_base_, GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV, GL_TRANSLATE_X_NV, kerning);

			//right
			string right_text;
			if (settings::markdown_type_ == 0)
				right_text = to_string(line->demerits().result);
			else if (settings::markdown_type_ == 1)
				right_text = to_string(line->demerits().r);
			else if (settings::markdown_type_ == 2)
				right_text = to_string(line->demerits().penalty);
			else if (settings::markdown_type_ == 3)
				right_text = to_string(line->space_r());
			else if (settings::markdown_type_ == 4)
				right_text = to_string(line->font_r());
			else if (settings::markdown_type_ == 5)
				right_text = to_string(line->letter_space_r());
			else
				right_text = ("");

			const char * right_str = right_text.c_str();
			translate(temp, settings::content_width_point() + settings::em_size_ / 2, 0);
			mul(result, mat, temp);
			MatrixLoadToGL(result);
			glColor3f(0.6, 0.2, 0);
			glStencilFillPathInstancedNV(right_text.length(), GL_UNSIGNED_BYTE, right_str, font_base_, GL_PATH_FILL_MODE_NV, 0xFF, GL_TRANSLATE_X_NV, kerning);
			glCoverFillPathInstancedNV(right_text.length(), GL_UNSIGNED_BYTE, right_str, font_base_, GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV, GL_TRANSLATE_X_NV, kerning);
		}

	}
	glMatrixPopEXT(GL_MODELVIEW);
	context_->swapBuffers(context_->surface());
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	initGraphics();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	slide_y = event->y();
	slide_x = event->x();
	event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	float m[2][3];

	float x_offset = event->x() - slide_x;
	float y_offset = event->y() - slide_y;
	translate(m, x_offset, y_offset);
	mul(view, m, view);
	slide_y = event->y();
	slide_x = event->x();
	update();
	event->accept();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
	if (event->modifiers() != Qt::ControlModifier)
	{
		event->ignore();
		return;
	}

	Transform3x2 s;
	float ratio;
	if (event->delta() > 0)
	{
		ratio = 1.1;
	}
	else
	{
		ratio = 1 / 1.1;
	}
	scale(s, ratio, ratio);
	mul(view, s, view);
	update();
	event->accept();
}

void GLWidget::identity(Transform3x2 dst)
{
	dst[0][0] = 1;
	dst[0][1] = 0;
	dst[0][2] = 0;
	dst[1][0] = 0;
	dst[1][1] = 1;
	dst[1][2] = 0;
}

void GLWidget::mul(Transform3x2 dst, Transform3x2 a, Transform3x2 b)
{
	Transform3x2 result;

	result[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
	result[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
	result[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2];

	result[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
	result[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];
	result[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2];

	dst[0][0] = result[0][0];
	dst[0][1] = result[0][1];
	dst[0][2] = result[0][2];
	dst[1][0] = result[1][0];
	dst[1][1] = result[1][1];
	dst[1][2] = result[1][2];
}

void GLWidget::translate(Transform3x2 dst, float x, float y)
{
	dst[0][0] = 1;
	dst[0][1] = 0;
	dst[0][2] = x;
	dst[1][0] = 0;
	dst[1][1] = 1;
	dst[1][2] = y;
}

void GLWidget::scale(Transform3x2 dst, float x, float y)
{
	dst[0][0] = x;
	dst[0][1] = 0;
	dst[0][2] = 0;

	dst[1][0] = 0;
	dst[1][1] = y;
	dst[1][2] = 0;
}

void GLWidget::rotate(Transform3x2 dst, float angle)
{
	float radians = angle*3.14159f / 180.0f,
		s = (float)sin(radians),
		c = (float)cos(radians);

	dst[0][0] = c;
	dst[0][1] = -s;
	dst[0][2] = 0;
	dst[1][0] = s;
	dst[1][1] = c;
	dst[1][2] = 0;
}

void GLWidget::ortho(Transform3x2 dst, float l, float r, float b, float t)
{
	dst[0][0] = 2 / (r - l);
	dst[0][1] = 0;
	dst[0][2] = -(r + l) / (r - l);
	dst[1][0] = 0;
	dst[1][1] = 2 / (t - b);
	dst[1][2] = -(t + b) / (t - b);
}

void GLWidget::inverse_ortho(Transform3x2 dst, float l, float r, float b, float t)
{
	dst[0][0] = (r - l) / 2;
	dst[0][1] = 0;
	dst[0][2] = (r + l) / 2;
	dst[1][0] = 0;
	dst[1][1] = (t - b) / 2;
	dst[1][2] = (t + b) / 2;
}

void GLWidget::xform(float dst[2], Transform3x2 a, const float v[2])
{
	float result[2];

	result[0] = a[0][0] * v[0] + a[0][1] * v[1] + a[0][2];
	result[1] = a[1][0] * v[0] + a[1][1] * v[1] + a[1][2];

	dst[0] = result[0];
	dst[1] = result[1];
}

void GLWidget::MatrixLoadToGL(Transform3x2 m)
{
	GLfloat mm[16];  /* Column-major OpenGL-style 4x4 matrix. */

	/* First column. */
	mm[0] = m[0][0];
	mm[1] = m[1][0];
	mm[2] = 0;
	mm[3] = 0;

	/* Second column. */
	mm[4] = m[0][1];
	mm[5] = m[1][1];
	mm[6] = 0;
	mm[7] = 0;

	/* Third column. */
	mm[8] = 0;
	mm[9] = 0;
	mm[10] = 1;
	mm[11] = 0;

	/* Fourth column. */
	mm[12] = m[0][2];
	mm[13] = m[1][2];
	mm[14] = 0;
	mm[15] = 1;

	glMatrixLoadfEXT(GL_MODELVIEW, &mm[0]);
}