#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


using namespace std;

class Volume
{

public:
    Volume();

public:
    bool init(QOpenGLShaderProgram *program, vector<int> v, int x, int y, int z);
	void destroy();

private:
    vector<int> voxels;
    int sX, sY, sZ;

};


#endif // TRIANGLEMESH_H
