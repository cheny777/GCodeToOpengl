#ifndef VIEWERTYPE_H
#define VIEWERTYPE_H
#include <vector>

using namespace std;

class Vertex {
    //定义三维图形的
    //用于face结构体中
public:
    float x, y, z;
} ;

class Face {
public:
    //多边形（三角形）面的结构体
    Face(void) : vert_number(0), verts(0) {};
    int vert_number;        //记录顶点的个数
    Vertex **verts;          //这是一个面的所有 顶点数组（含有坐标）
    float normal[3];         //记录点的法向量，分别是x，y，z三个方向
    //注意点的法向量通过顶点的信息计算得到！
    //对于obj模型如果我们已经得到了法线的信息
    //那么就直接拿来用就好！
} ;

class ViewMesh {
    //自定义mesh的结构体（三角面片模型结构体）
public:
    ViewMesh(void) : vert_number(0), verts(0), face_number(0), faces(0) {mesh_color[0] = 0;mesh_color[1] = 1;mesh_color[2] = 0;};
    //自定义构造器
    int vert_number;        //总的顶点个数
    Vertex *verts;             //定点数组
    int face_number;                 //面的数目
    Face *faces;    /*面数组*/
    vector<Vertex> point; /*点容器*/
    float mesh_color[3]; /*颜色*/
};

class ViewLine
{
public:
    ViewLine():line_number(0){line_color[0] = 0;line_color[1] = 0;line_color[2] = 1;}
    int line_number;
    vector<Vertex> point; /*点容器*/
    float line_color[3]; /*线颜色*/

};

class ViewPoint
{
public:
    ViewPoint():point_number(0){line_color[0] = 1;line_color[1] = 0;line_color[2] = 0;}
    int point_number;
    vector<Vertex> point; /*点容器*/
    float line_color[3]; /*颜色*/

};



#endif // VIEWERTYPE_H
