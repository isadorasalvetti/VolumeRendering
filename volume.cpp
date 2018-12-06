#include <iostream>
#include "volume.h"


using namespace std;

Volume::Volume(){
}

bool Volume::init(QOpenGLShaderProgram *program, vector<int> v, int x, int y, int z){
    voxels = v;
    sX = x;
    sY = y;
    sZ = z;
}

array<int, 3> getVoxel(int id){

}

int getId(array<int, 3>){

}




