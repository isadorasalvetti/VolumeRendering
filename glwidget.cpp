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
    glEnable(GL_TEXTURE_3D);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0,0,w,h);
    program->bind();
    program->setUniformValue("SIZE", w, h);
    program->release();
//	setProjection((float)w/h);
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
    setViewDirection();
	doneCurrent();
	update();
}

void GLWidget::setViewDirection()
{
    QMatrix4x4 ViewMatrix;
    ViewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
    ViewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
    QVector3D rayDirection = QVector3D(ViewMatrix.inverted()*QVector4D(0,0,-1,0));
	program->bind();
    program->setUniformValue("rayDirection", rayDirection);
	program->release();
}

QVector3D GLWidget::getEntryPosition(QVector3D direction){
    //Cube vertices
    QVector3D candidateVertices[8] = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 0, 1),
        QVector3D(1, 1, 0),
        QVector3D(0, 1, 1),
        QVector3D(1, 0, 1),
        QVector3D(1, 1, 1),
    };

    QVector3D candidateNormals[6] = {
        QVector3D(1, 0, 0),
        QVector3D(0, 1, 0),
        QVector3D(0, 0, 1),
        QVector3D(-1, 0, 0),
        QVector3D(0, -1, 0),
        QVector3D(0, 0, -1)
    };

    //Find closest face
    float highestAngle = 0;
    int closestFace;
    for (int i = 0; i < 6; i++){
        float angle = QVector3D::dotProduct(direction, candidateNormals[i].normalized());
        if (angle > highestAngle){
            highestAngle = angle;
            closestFace = i;
        }
    }

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
