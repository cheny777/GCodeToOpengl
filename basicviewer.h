#ifndef BASICVIEWER_H
#define BASICVIEWER_H

#include <QWidget>
#include <QGLViewer/qglviewer.h>
#include "viewertype.h"
#include "modelfileread.h"
#include <QList>
#include <QVector3D>
class Scene;
class BasicViewer :public QGLViewer
{
    Q_OBJECT
public:
    BasicViewer(QWidget *parent = NULL);
    ~BasicViewer();

    /*添加*/
    void adddata(ViewMesh *);
    /*添加*/
    void adddata(ViewLine *);
    /*添加*/
    void adddata(ViewPoint *);
    /*添加*/
    void adddata(QList<QVector3D> list);
    /*清空*/
    void clear();

    /*更新*/
    void updateV();
protected:
    virtual void draw();
    virtual void init();
    void initializeGL();
private:
    /*绘制面*/
    void drawmesh();
    /*绘制线*/
    void drawLine();
    /*绘制点*/
    void drawpoint();

    /*圆弧*/
    void drawarc();
    vector<ViewMesh *> m_vmesh;
    vector<ViewLine *> m_vline;
    vector<ViewPoint *> m_vpoint;

    void setScene(Scene* pScene);
    bool m_custom_mouse;

    qglviewer::Vec m_vcenter;

protected:
  virtual void mousePressEvent(QMouseEvent* e);
  virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    void drawAxisxyz(qglviewer::Vec v);
};

#endif // BASICVIEWER_H
