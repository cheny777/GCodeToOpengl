#ifndef MODELFILEREAD_H
#define MODELFILEREAD_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>
#include <assert.h>
#include <QtOpenGL/qgl.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "viewertype.h"
using namespace std;


/*模型文件处理*/
class ModelFileRead
{
public:
    ModelFileRead();
    ModelFileRead(string filename);
    /*读取模型*/
    void read(string filename);
    /*获取模型数据*/
    ViewMesh* getModel();
    /*读取是否成功*/
    bool is_read();
private:
    /*STL文件解析*/
    void ReadASCII(const char *cfilename);
    void ReadBinary(const char *cfilename);
    void ReadSTLFile(const char *cfilename);

    /*obj文件解析*/
    void ReaderOBj(string fname2);
    /*off文件解析*/
    void ReadOffFile(const char *filename);
    void get_normal(Face& face);


    ViewMesh *m_mesh;
    /*是否读取成功*/
    bool m_bread;
};


class NCfileread
{
public:
    NCfileread();
    void read(string filename );

    ViewLine *getLine();
    bool is_read();
private:
    void readNCG0(const char * str);

    void readall(const char * str);




    ViewLine *m_line;
    bool m_bread;
};






#endif // MODELFILEREAD_H
