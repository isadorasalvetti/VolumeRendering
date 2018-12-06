#ifndef PLYREADER_H
#define PLYREADER_H

#include <exception>
#include <fstream>
#include "volume.h"


class sizeException:public exception{
  public:
    const string Error(){
        return "Read size does not match expected resolution."
               "\nSize: " + to_string(size) + "\nExpected Size:" + to_string(expectedSize);
    }
    int size, expectedSize;
    void SetSize(int s, int es){
        size = s;
        expectedSize = es;
    }
};

class RawReader
{

public:
    vector<int> readVolume(const QString &filename, int expectedSize);

private:
    static void loadVolume(vector<int> &voxels, ifstream &fin);
    static void VerifySize(int mySize, int expectedSize);
};

#endif // PLYREADER_H
