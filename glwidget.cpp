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
    ViewMatrix.translate(0.0f, 0.0f, -distance);
    QMatrix4x4 invViewMatrix = ViewMatrix.inverted();
    QVector3D rayDirection = QVector3D(invViewMatrix*QVector4D(0,0,-1,1));
    //QVector3D closestFaces = setCandidatesForEntryPosition(rayDirection);
	program->bind();
    program->setUniformValue("viewMatrix", ViewMatrix);
    program->setUniformValue("invViewMatrix", invViewMatrix);
    program->setUniformValue("worldSpaceRayDirection", rayDirection);
    //program->setUniformValue("candidateFaces", closestFaces);
	program->release();
}

QVector3D GLWidget::setCandidatesForEntryPosition(const QVector3D direction){
    //Cube vertices
    QVector3D candidateNormals[6] = {
        QVector3D(0, 0, -1), //1. front
        QVector3D(-1, 0, 0), //2. right
        QVector3D(0, -1, 0), //3. top
        QVector3D(0, 0, 1), //4. back
        QVector3D(1, 0, 0), //5. left
        QVector3D(0, 1, 0) //6. bottom
    };

    //Find closest faces
    float highestAngle = 0;
    vector<pair<int, float>> closestFaces; closestFaces.reserve(3);
    for (int i = 0; i < 6; i++){
        float angle = QVector3D::dotProduct(direction, candidateNormals[i].normalized());
            if (closestFaces.size() < 3) closestFaces.push_back(pair<int, float>(i, angle));

            else {
                if (angle > closestFaces[0].second){
                    closestFaces.pop_back();
                    closestFaces.insert(closestFaces.begin(), pair<int, float>(i, angle));
                }
                else if (angle > closestFaces[1].second){
                    closestFaces.pop_back();
                    closestFaces.insert(closestFaces.begin()+1, pair<int, float>(i, angle));
                }
                else if (angle > closestFaces[2].second){
                    closestFaces.pop_back();
                    closestFaces.insert(closestFaces.end(), pair<int, float>(i, angle));
                }
            }
        }

    return QVector3D(closestFaces[0].first, closestFaces[1].first, closestFaces[2].first);
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
