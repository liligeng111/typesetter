#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "nvpr_init.h"
#include "xform.h"
#include "container.h"

class NVPR_Page
{
public:
	NVPR_Page();
	~NVPR_Page();
	
	void render_page(Page* page, int page_num);
	void start_main_loop() {glutMainLoop();}

private:
	static NVPR_Page* instance_;

	GLuint font_base_;
	GLuint path_base_;
	unsigned int path_count_;

	int width, height;
	static void display();
	static void reshape(int w, int h);
	static void	keyboard(unsigned char c, int x, int y);
	static void	mouse(int button, int state, int mouse_space_x, int mouse_space_y);
	static void	wheel(int wheel, int direction, int x, int y);
	static void	motion(int mouse_space_x, int mouse_space_y);
	static void	menu(int choice);
	void fatalError(const char *message);
	void initGraphics();
	void initModelAndViewMatrices();

	const char *programName = "nvpr_page_renderer";

	/* Scaling and rotation state. */
	int anchor_x = 0, anchor_y = 0;  /* Anchor for rotation and scaling. */
	int zoom = 0;

	/* Sliding (translation) state. */
	float slide_x = 0, slide_y = 0;  /* Prior (x,y) location for sliding. */
	int sliding = 0;  /* Are we sliding currently? */

	Transform3x2 model, view;

	//page rendering
	Page* page_;
	//TODO::ugly
	vector<int> line_num_;
	vector<int> char_num_;
};

