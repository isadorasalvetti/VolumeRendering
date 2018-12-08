#include <iostream>
#include <algorithm>
#include "volumerender.h"

using namespace std;

VolumeRender::VolumeRender(QOpenGLFunctions_3_3_Core &ogl)
    : VBOfaces(QOpenGLBuffer::IndexBuffer)
    , gl(ogl)
{
}

bool VolumeRender::init(QOpenGLShaderProgram *program){
    //BuildBuffers(program);
    //BuildPlane();

    static float vertices[4][3] = {
        {1.0, -1.0, 0.0},
        {-1.0, -1.0, 0.0},
        {-1.0, 1.0, 0.0},
        {1.0, 1.0, 0.0}
    }; // CW from bottom-right

    static unsigned short faces[6] = {
        0, 1, 2,
        0, 2, 3
    };
    VAO.create();
    VAO.bind();

    program->bind();
    VBOverts.create();
    VBOverts.bind();
    VBOverts.setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBOverts.allocate(vertices, 12*sizeof(float));

    program->enableAttributeArray(0);
    program->setAttributeArray(0, GL_FLOAT, nullptr, 3, 0);

    VBOfaces.create();
    VBOfaces.bind();
    VBOfaces.setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBOfaces.allocate(faces, 6*sizeof(unsigned short));

    VAO.release();
    program->release();
    return false;
}

bool VolumeRender::setVolumeData(QOpenGLShaderProgram *program, vector<unsigned short> v, int x, int y, int z){
    voxels = v;

    cout << "Processing texture" << endl;

//    unsigned short sMax = 0;
//    unsigned short sMin = 32768;

//    for (unsigned int i =0; i< voxels.size(); i++){
//        sMax = max(sMax, v[i]);
//        sMin = min(sMin, v[i]);
//    }
//    for (unsigned int i =0; i< voxels.size(); i++) voxels[i] = (v[i] - sMin)/(float)(sMax-sMin);

    cout << "Loading 3D texture" << endl;

    gl.glGenTextures(1, &volumeTexture);
    gl.glBindTexture(GL_TEXTURE_3D, volumeTexture);

    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    gl.glEnable(GL_TEXTURE_3D);
    gl.glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, x, y, z, 0, GL_RED, GL_UNSIGNED_SHORT, &voxels[0]);

    gl.glBindTexture(GL_TEXTURE_3D, 0);

    program->bind();
    program->setUniformValue("VOXELS", 0);
    program->release();

    voxelsLoaded = true;
    return true;
}

void VolumeRender::render(){
    if (voxelsLoaded){
        VAO.bind();
        gl.glActiveTexture(GL_TEXTURE0);
        gl.glBindTexture(GL_TEXTURE_3D, volumeTexture);
        gl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
        VAO.release();
    }
}

void VolumeRender::DefUniforms(QOpenGLShaderProgram *program){
    program->bind();

    program->release();
}


