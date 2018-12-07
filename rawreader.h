#ifndef PLYREADER_H
#define PLYREADER_H

#include <exception>
#include <fstream>
#include <iostream>
#include "volumerender.h"

template <typename T>
class RawReader {

public:
    RawReader(const QString &filename, int expectedSize){
        data.reserve(expectedSize);
        ifstream fin;
        fin.open(filename.toStdString().c_str(), ios_base::in | ios_base::binary);
        if(!fin.is_open())
            throw runtime_error("Can not open file " + filename.toStdString());

        loadVolume(data, fin);

        fin.close();

        verifySize(data.size(), expectedSize);
    }

    const vector<T>& getData() const {
        return data;
    }

private:
    class sizeException:public exception{
      public:
        const string Error(){
            return "Read size does not match expected resolution."
                   "\nSize: " + to_string(size) + "\nExpected Size:" + to_string(expectedSize);
        }
        int size, expectedSize;
        void setSize(int s, int es){
            size = s;
            expectedSize = es;
        }
    };

    vector<T> data;
    static void loadVolume(vector<T> &voxels, ifstream &fin){
        T val;
        while (fin and not fin.eof()){
            fin.read(reinterpret_cast<char *>(&val), sizeof(val));
            if (fin.good())
                voxels.push_back(val);
        }
    }

    static void verifySize(int mySize, int expectedSize){
    if (mySize != expectedSize){
        sizeException e;
        e.setSize(mySize, expectedSize);
        throw e;
    }

}
};

#endif // PLYREADER_H
