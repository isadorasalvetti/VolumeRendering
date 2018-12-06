#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "volume.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
	GLWidget(QWidget *parent);
	~GLWidget();

    void loadMesh(const QString &filename, int x, int y, int z);
	void setPolygonMode(bool bFill);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void setProjection(float aspect);
	void setModelview();

private:
	bool bPolygonFill;
	float angleX, angleY, distance;
	QPoint lastMousePos;

	QOpenGLShaderProgram *program;
    Volume vol;

};

#endif // GLWIDGET_H
