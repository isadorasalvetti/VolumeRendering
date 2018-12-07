#include <iostream>
#include "volumerender.h"

using namespace std;

VolumeRender::VolumeRender(QOpenGLFunctions_3_3_Core &ogl)
    : VBOfaces(QOpenGLBuffer::IndexBuffer)
    , gl(ogl)
{
}

bool VolumeRender::init(QOpenGLShaderProgram *program){
    BuildBuffers(program);
    BuildPlane();

    return false;
}

bool VolumeRender::setVolumeData(QOpenGLShaderProgram *program, vector<unsigned short> v, int x, int y, int z){

    voxels = v;
    sX = x;
    sY = y;
    sZ = z;

    Create3Dtexture(program);
    voxelsLoaded = true;
    return true;
}

void VolumeRender::render(){
    if (voxelsLoaded){
        VAO.bind();
        gl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        VAO.release();
    }
}

void VolumeRender::Create3Dtexture(QOpenGLShaderProgram *program){
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint volumeTexture;
    gl.glGenTextures(1, &volumeTexture);
    gl.glBindTexture(GL_TEXTURE_3D, volumeTexture);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gl.glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    gl.glTexImage3D(GL_TEXTURE_3D, 0, GL_R16UI, sX, sY, sZ, 0, GL_RED, GL_UNSIGNED_SHORT, &voxels);

    program->bind();
    program->setUniformValue("volumeLoaded", true);
    program->setUniformValue("VOXELS", volumeTexture);

}

void VolumeRender::DefUniforms(QOpenGLShaderProgram *program){
    program->bind();

}

void VolumeRender::BuildPlane(){
    //Lists of vertices, faces and normal
    GLint vertices[12] = {
        1, -1, 0,
        -1, -1, 0,
        -1, 1, 0,
        1, 1, 0
    };
    GLuint faces[6] = {
        0, 1, 2,
        1, 2, 3
    };

    //Alocate buffer data
    VAO.bind();
    VBOverts.bind();
    VBOverts.allocate(&vertices[0], 3 * sizeof(GLint) * 12);
    VBOverts.release();

    VBOfaces.bind();
    VBOfaces.allocate(&faces[0], 3 * sizeof(GLuint) * 6);
    VBOfaces.release();
    VAO.release();

    if (!VBOfaces.isCreated() || !VBOverts.isCreated() || !VAO.isCreated())
        throw runtime_error("Vertex Buffers creation failed");
}

void VolumeRender::BuildBuffers(QOpenGLShaderProgram *program){
   VAO.create();
   VBOverts.create(); VBOfaces.create();

   program->bind();
   VAO.bind();

   VBOverts.bind();
   VBOverts.setUsagePattern(QOpenGLBuffer::StaticDraw);
   program->enableAttributeArray(0);
   program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

   VBOfaces.bind();
   VBOfaces.setUsagePattern(QOpenGLBuffer::StaticDraw);
   program->release();
}



