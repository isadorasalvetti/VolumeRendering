#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "volumerender.h"

enum shader{TRANSPARENT, OPAQUE};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{

public:
	GLWidget(QWidget *parent);
	~GLWidget();

    void loadMesh(const QString &filename, int x, int y, int z);
    void updateParameters(float brightness, float min, float max);
    void changeShader(shader type);

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
    QVector2D currentSize;
    bool bPolygonFill;
	float angleX, angleY, distance;
	QPoint lastMousePos;

	QOpenGLShaderProgram *program;
    QOpenGLShader *vs, *fs_opaque, *fs_transparent;
    VolumeRender vol;

};

#endif // GLWIDGET_H
