#include <iostream>
#include <vector>
#include "rawreader.h"

using namespace std;

vector<int> RawReader::readVolume(const QString &filename, int expectedSize)
{
	ifstream fin;
    vector<int> voxels;

	fin.open(filename.toStdString().c_str(), ios_base::in | ios_base::binary);
	if(!fin.is_open())
        return voxels;

    loadVolume(voxels, fin);
	fin.close();

    try {
        VerifySize(voxels.size(), expectedSize);
    } catch (sizeException e) {
        cout << e.Error() << endl;
    }

    return voxels;
}

void RawReader::loadVolume(vector<int> &voxels, ifstream &fin){
    short val;
    while (fin.good()){
        fin.read(reinterpret_cast<char *>(&val), sizeof(val));
        voxels.push_back(val);
    }
}

void RawReader::VerifySize(int mySize, int expectedSize){
    if (mySize != expectedSize+1){
        sizeException e;
        e.SetSize(mySize, expectedSize);
        throw e;
    }
}

