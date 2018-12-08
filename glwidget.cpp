#include "glwidget.h"
#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <rawreader.h>


using namespace std;


const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 3.0f;


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , bPolygonFill(true)
    , angleX(0.0f)
    , angleY(0.0f)
    , distance(2.0f)
    , vol(*this)
{
    program = nullptr;
}

GLWidget::~GLWidget()
{
	if(program) delete program;
}


void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	program = new QOpenGLShaderProgram();
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
	program->link();
	if(!program->isLinked())
	{
			cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
			QApplication::quit();
	}
	program->bind();

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    program->setUniformValue("SIZE", m_viewport[2], m_viewport[3]);

    vol.init(program);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0,0,w,h);
//	setProjection((float)w/h);
//	setModelview();
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program->bind();
    vol.render();
	program->release();
}

/*
void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Rotation
	if(event->buttons() & Qt::LeftButton)
	{
		angleX += rotationFactor * (event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor * (event->x() - lastMousePos.x());
	}
	// Zoom
	if(event->buttons() & Qt::RightButton)
	{
		distance += 0.01f * (event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

	lastMousePos = event->pos();

	makeCurrent();
	setModelview();
	doneCurrent();
	update();
}

void GLWidget::setProjection(float aspect)
{
	QMatrix4x4 projectionMatrix;

	projectionMatrix.perspective(60, aspect, 0.01, 100.0);
	program->bind();
	program->setUniformValue("projection", projectionMatrix);
	program->release();
}

void GLWidget::setModelview()
{
	QMatrix4x4 modelviewMatrix;

	modelviewMatrix.translate(0, 0, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
	program->bind();
	program->setUniformValue("modelview", modelviewMatrix);
	program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
	program->release();
}

void GLWidget::setPolygonMode(bool bFill)
{
	bPolygonFill = bFill;

	makeCurrent();
	if(bFill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	doneCurrent();
	update();
}
*/
void GLWidget::loadMesh(const QString &filename, int x, int y, int z)
{
    cout << "Loading data of resolution " << x << ", " << y << ", " << z << "." << endl;
    RawReader<unsigned short> reader(filename, x*y*z);

    if(!vol.setVolumeData(program, reader.getData(), x, y, z))
	{
            cout << "Could not initialize voxel rendering" << endl;
			QApplication::quit();
	}
	doneCurrent();
	update();

}
