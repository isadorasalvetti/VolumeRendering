#ifndef VOLUMERENDER_H
#define VOLUMERENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>


using namespace std;

class VolumeRender
{

public:
    VolumeRender(QOpenGLFunctions_3_3_Core &ogl);

public:
    bool init(QOpenGLShaderProgram *program);
    bool setVolumeData(QOpenGLShaderProgram *program, vector<unsigned short> v, int x, int y, int z);
	void destroy();
    void render();

private:
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBOverts, VBOfaces;
    void BuildBuffers(QOpenGLShaderProgram *program);
    void BuildPlane();
    void Create3Dtexture(QOpenGLShaderProgram *program);
    void DefUniforms(QOpenGLShaderProgram *program);

    bool voxelsLoaded = true;
    vector<unsigned short> voxels;
    int sX, sY, sZ;

    QOpenGLFunctions_3_3_Core &gl;
};


#endif // VOLUMERENDER_H


