#include "nvpr_page.h"
#include <vector>
#include "settings.h"

NVPR_Page* NVPR_Page::instance_ = nullptr;

NVPR_Page::NVPR_Page()
{
	instance_ = this;
	page_ = nullptr;

	path_base_ = 0;
	font_base_ = 0;
	path_count_ = 0;
	width = 630;
	height = 891;

	GLenum status;
	GLboolean hasDSA;

	glutInitWindowSize(width, height);
	int* argc = new int(0);
	char** argv = {};
	glutInit(argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glEnable(GL_MULTISAMPLE);


	glutCreateWindow("Page Viewer");
	// detect current settings
	printf("samples per pixel = %d\n", glutGet(GLUT_WINDOW_NUM_SAMPLES));

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(wheel);

	initModelAndViewMatrices();

	glutCreateMenu(menu);
	glutAddMenuEntry("[r] Reset view", 'r');
	glutAddMenuEntry("[Esc] Quit", 27);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	status = glewInit();
	if (status != GLEW_OK)
	{
		fatalError("OpenGL Extension Wrangler (GLEW) failed to initialize");
	}
	hasDSA = glewIsSupported("GL_EXT_direct_state_access");
	if (!hasDSA)
	{
		fatalError("OpenGL implementation doesn't support GL_EXT_direct_state_access (you should be using NVIDIA GPUs...)");
	}

	initializeNVPR(programName);
	if (!has_NV_path_rendering)
	{
		fatalError("required NV_path_rendering OpenGL extension is not present");
	}
}


NVPR_Page::~NVPR_Page()
{
	
}

void NVPR_Page::render_page(Page* page, int page_num)
{
	string title = "Page: " + to_string(page_num);
	glutSetWindowTitle(title.c_str());
	page_ = page;
	//delete previous page
	if (path_count_ != 0)
		glDeletePathsNV(path_base_, path_count_);
	path_count_ = 0;
	path_base_ = 0;
	initGraphics();
	glutPostRedisplay();

}

void NVPR_Page::fatalError(const char *message)
{
	fprintf(stderr, "%s: %s\n", programName, message);
	exit(1);
}

void NVPR_Page::initGraphics()
{
	/* Use an orthographic path-to-clip-space transform to map the
	[0..640]x[0..480] range of the star's path coordinates to the [-1..1]
	clip space cube: */
	glMatrixLoadIdentityEXT(GL_PROJECTION);
	glMatrixOrthoEXT(GL_PROJECTION, 0, width, height, 0, -1, 1);
	glMatrixLoadIdentityEXT(GL_MODELVIEW);

	/* Before rendering to a window with a stencil buffer, clear the stencil
	buffer to zero and the color buffer to blue: */
	glClearStencil(0);
	glClearColor(1, 1, 1, 0.0);

	const int numChars = 256; // ISO/IEC 8859-1 8-bit character range
	GLuint templatePathObject = ~0; // Non-existant path object
	const GLfloat emScale = 2048; // match TrueType convention
	font_base_ = glGenPathsNV(numChars);
	glPathGlyphRangeNV(font_base_, GL_SYSTEM_FONT_NAME_NV, "Helvetica", GL_BOLD_BIT_NV, 0, numChars, GL_SKIP_MISSING_GLYPH_NV, templatePathObject, emScale);

	unsigned int i;
	vector<const char*> paths;
	line_num_.clear();
	char_num_.clear();
	for (int l = 0; l < page_->children_size(); l++)
	{
		Line* line = static_cast<Line*>(page_->child(l));
		for (int c = 0; c < line->children_size(); c++)
		{
			//TODO::break glyph
			if (line->child(c)->glyph() != nullptr)
			{
				const char* svg_str = line->child(c)->glyph()->path()->c_str();
				paths.push_back(svg_str);
				//printf(svg_str);
				//cout << std_str << endl;
				line_num_.push_back(l);
				char_num_.push_back(c);
			}
			else if (line->child(c)->is_break() && line->child(c)->break_glyph() != nullptr)
			{
				const char* svg_str = line->child(c)->break_glyph()->path()->c_str();
				paths.push_back(svg_str);
				//printf(svg_str);
				//cout << std_str << endl;
				line_num_.push_back(l);
				char_num_.push_back(c);
			}
		}
	}

	path_count_ = paths.size();
	path_base_ = glGenPathsNV(path_count_);

	for (i = 0; i < path_count_; i++) 
	{
		const char *svg_str = paths[i];
		size_t svg_len = strlen(paths[i]);
		GLfloat stroke_width = 1;

		glPathStringNV(path_base_ + i, GL_PATH_FORMAT_SVG_NV, (GLsizei)svg_len, svg_str);
		glPathParameterfNV(path_base_ + i, GL_PATH_STROKE_WIDTH_NV, stroke_width);
	}

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
	glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
}

void NVPR_Page::initModelAndViewMatrices()
{
	float tmp[2][3];
	
	translate(tmp, settings::mm_to_point(settings::margin_left_), settings::mm_to_point(settings::margin_top_));
	scale(model, 3 / settings::mm_to_point(1), 3 / settings::mm_to_point(1));
	mul(model, model, tmp);
	translate(view, 0, 0);
}

void NVPR_Page::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixPushEXT(GL_MODELVIEW); 
	{
		for (int i = 0; i < instance_->path_count_; i++)
		{
			Transform3x2 mat;
			mul(mat, instance_->view, instance_->model);

			Transform3x2 result;

			Line* line = static_cast<Line*>(instance_->page_->child(instance_->line_num_[i]));
			Transform3x2 line_trans;
			translate(line_trans, line->x(), line->y());
			Transform3x2 temp;
			scale(temp, 1, -1);
			mul(line_trans, line_trans, temp);
			mul(result, mat, line_trans);
			
			Item* item = line->child(instance_->char_num_[i]);
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
				float r = line->demerits().r;
				if (r < 0)
				{
					r /= -7;
					if (r > 1)
						r = 1;
					glColor3f(1, 0.4, 0);
				}
				else
				{
					r /= 7;
					if (r > 1)
						r = 1;
					glColor3f(1, 0.4, 0);
				}
			}
			else
			{
				glColor3f(0, 0, 0);
			}
			GLuint stroke_color = 0x000000;
			GLfloat stroke_width = 1;

			glStencilFillPathNV(instance_->path_base_ + i, GL_COUNT_UP_NV, 0x1F);
			glCoverFillPathNV(instance_->path_base_ + i, GL_BOUNDING_BOX_NV);

			GLfloat kerning[6 + 1]; 
			for (int i = 0; i < 7; i++)
				kerning[i] = 0;
			//glStencilFillPathInstancedNV(6, GL_UNSIGNED_BYTE, "\000\001\002\003\004\005", instance_->font_base_, GL_PATH_FILL_MODE_NV, 0xFF, GL_TRANSLATE_X_NV, kerning);
			//glCoverFillPathInstancedNV(6, GL_UNSIGNED_BYTE, "\000\001\002\003\004\005", instance_->font_base_, GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV, GL_TRANSLATE_X_NV, kerning);
			//glStencilFillPathInstancedNV(6, instance_->font_base_, GL_UNSIGNED_BYTE, "OpenGL", GL_PATH_FILL_MODE_NV, 0xFF, GL_TRANSLATE_X_NV, kerning);
			//glCoverFillPathInstancedNV(6, instance_->font_base_, GL_UNSIGNED_BYTE, "OpenGL", GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV, GL_TRANSLATE_X_NV, kerning);

		}
	} 
	glMatrixPopEXT(GL_MODELVIEW);
	glutSwapBuffers();
}

void NVPR_Page::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	instance_->width = w;
	instance_->height = h;
	instance_->initGraphics();
}

void NVPR_Page::keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 27:  /* Esc quits */
		exit(0);
		return;
	case 13:  /* Enter redisplays */
		break;
	case 'r':
		instance_->initModelAndViewMatrices();
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void NVPR_Page::mouse(int button, int state, int mouse_space_x, int mouse_space_y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			instance_->slide_y = mouse_space_y;
			instance_->slide_x = mouse_space_x;
			instance_->sliding = 1;
		}
		else
		{
			instance_->sliding = 0;
		}
	}
}

void NVPR_Page::wheel(int wheel, int direction, int x, int y)
{
	Transform3x2 s;
	float ratio;
	if (direction < 0)
	{
		ratio = 1.1;
	}
	else
	{
		ratio = 1 / 1.1;
	}
	scale(s, ratio, ratio);
	mul(instance_->view, s, instance_->view);
	glutPostRedisplay();
}

void NVPR_Page::motion(int mouse_space_x, int mouse_space_y)
{
	if (instance_->sliding)
	{
		float m[2][3];

		float x_offset = mouse_space_x - instance_->slide_x;
		float y_offset = mouse_space_y - instance_->slide_y;
		translate(m, x_offset, y_offset);
		mul(instance_->view, m, instance_->view);
		instance_->slide_y = mouse_space_y;
		instance_->slide_x = mouse_space_x;
		glutPostRedisplay();
	}
}

void NVPR_Page::menu(int choice)
{
	keyboard(choice, 0, 0);
}