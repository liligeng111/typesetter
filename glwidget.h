#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "extension_loader.h"
#include "container.h"

typedef float Transform3x2[2][3];

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;

	void render_page(Page* page);

	void resize(int width, int height) { resizeGL(width, height); };

public slots:
	void cleanup();

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;

private:
	QWidget* parent_;
	GLuint pathObj;

	void initGraphics();
	void initModelAndViewMatrices();

	QOpenGLContext* context_;
	GLuint font_base_;
	GLuint path_base_;
	unsigned int path_count_;

	int width, height;
	
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

	//xform
	void identity(Transform3x2 dst);
	void mul(Transform3x2 dst, Transform3x2 a, Transform3x2 b);
	void translate(Transform3x2 dst, float x, float y);
	void scale(Transform3x2 dst, float x, float y);
	void rotate(Transform3x2 dst, float angle);
	void ortho(Transform3x2 dst, float l, float r, float b, float t);
	void inverse_ortho(Transform3x2 dst, float l, float r, float b, float t);
	void xform(float dst[2], Transform3x2 a, const float v[2]);
	void MatrixLoadToGL(Transform3x2 m);
};
