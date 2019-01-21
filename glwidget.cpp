#include "glwidget.h"
#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <rawreader.h>
#include <set>


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
    , distance(0.0f)
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

    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    fs_opaque = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs_transparent = new QOpenGLShader(QOpenGLShader::Fragment, this);

    if (!vs->compileSourceFile(":/shaders/simpleshader.vert"))
        cout << "vs log:" << vs->log().toStdString() << endl;
    fs_opaque->compileSourceFile(":/shaders/opaque.frag");
    fs_transparent->compileSourceFile(":/shaders/xRayShader.frag");

	program = new QOpenGLShaderProgram();
    program->addShader(vs);
    program->addShader(fs_transparent);
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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_3D);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0,0,w,h);
    currentSize = QVector2D(w, h);
    program->bind();
    program->setUniformValue("SIZE", w, h);
    program->release();
    setViewDirection();
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program->bind();
    vol.render();
	program->release();
}


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
        //angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
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
    setViewDirection();
	doneCurrent();
	update();
}

void GLWidget::setViewDirection()
{
    QMatrix4x4 ViewMatrix;
    ViewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
    ViewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
    ViewMatrix.translate(0.0f, 0.0f, 0.0f);
    QMatrix4x4 invViewMatrix = ViewMatrix.inverted();

	program->bind();
    program->setUniformValue("viewMatrix", ViewMatrix);
    program->setUniformValue("invViewMatrix", invViewMatrix);
	program->release();
}

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

void GLWidget::updateParameters(float brightness, float min, float max){
    program->bind();
    program->setUniformValue("brightness", brightness);
    program->setUniformValue("min", min);
    program->setUniformValue("max", max);
    program->release();
}

void GLWidget::changeShader(shader type){
    program->removeAllShaders();

    program->addShader(vs);
    if (type == TRANSPARENT) program->addShader(fs_transparent);
    else if (type == OPAQUE) program->addShader(fs_opaque);

    program->link();
    if(!program->isLinked())
    {
            cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
            QApplication::quit();
    }
    program->bind();
    program->setUniformValue("SIZE", currentSize[0], currentSize[1]);
    program->release();
    if (vol.voxelsLoaded)vol.bindtoProgram(program);
    setViewDirection();
}
