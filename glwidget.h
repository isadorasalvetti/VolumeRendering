#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "volumerender.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{

public:
	GLWidget(QWidget *parent);
	~GLWidget();

    void loadMesh(const QString &filename, int x, int y, int z);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void setViewDirection();
    QVector3D setCandidatesForEntryPosition(const QVector3D direction);

private:
    bool bPolygonFill;
	float angleX, angleY, distance;
	QPoint lastMousePos;

	QOpenGLShaderProgram *program;
    VolumeRender vol;

};

#endif // GLWIDGET_H
