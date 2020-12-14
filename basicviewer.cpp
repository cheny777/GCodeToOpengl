#include "basicviewer.h"
#include <constraint.h>
#include "manipulatedCameraFrame.h"
BasicViewer::BasicViewer(QWidget *parent)
{
    //this->setAxisIsDrawn(true);
    this->showEntireScene();

    m_vcenter.setValue(0,0,0);
}

BasicViewer::~BasicViewer()
{

}

void BasicViewer::adddata(ViewMesh *v)
{
    m_vmesh.push_back(v);
}

void BasicViewer::adddata(ViewLine *v)
{
    m_vline.push_back(v);
}

void BasicViewer::adddata(ViewPoint *v)
{
    m_vpoint.push_back(v);
}

void BasicViewer::adddata(QList<QVector3D> list)
{
    ViewLine *line = new ViewLine();
    double fx = 0,fy = 0,fz=0;
    for (int i=0;i<list.size();i++)
    {
        Vertex v;
        v.x = list[i].x();
        v.y = list[i].y();
        v.z = list[i].z();
        fx+=v.x;
        fy+=v.y;
        fz+=v.z;
        line->point.push_back(v);
    }
    line->line_number = list.size();
    m_vline.push_back(line);

    fx = fx/double(line->line_number);
    fy = fy/double(line->line_number);
    fz = fz/double(line->line_number);

    m_vcenter.setValue(fx,fy,fz);
    this->setSceneCenter(m_vcenter);

    qreal max[3][4];
    /*位置*/
    max[0][0] = m_vcenter[0];
    max[0][1] = m_vcenter[1];
    max[0][2] = m_vcenter[2];


    qglviewer::Vec pos = m_vcenter;
    pos[2] += 10;
    camera()->setPosition(pos);
    camera()->lookAt(m_vcenter);
}

void BasicViewer::clear()
{
    m_vline.clear();
    m_vmesh.clear();
    m_vpoint.clear();
}

void BasicViewer::updateV()
{
    draw();
}

void BasicViewer::draw()
{
    //drawAxis(1.0);
    drawmesh();
    drawLine();
    drawpoint();
    // drawarc();

    /*绘制坐标轴*/
    drawAxisxyz(m_vcenter);

}

void BasicViewer::init()
{
    //restoreStateFromFile();


    //    setMouseBinding(Qt::AltModifier, Qt::LeftButton, QGLViewer::CAMERA,
    //                    QGLViewer::ROTATE);
    //    setMouseBinding(Qt::AltModifier, Qt::RightButton, QGLViewer::CAMERA,
    //                    QGLViewer::TRANSLATE);
    //    setMouseBinding(Qt::AltModifier, Qt::MidButton, QGLViewer::CAMERA,
    //                    QGLViewer::ZOOM);
    //    setWheelBinding(Qt::AltModifier, QGLViewer::CAMERA, QGLViewer::ZOOM);

    //    setMouseBinding(Qt::NoModifier, Qt::LeftButton, QGLViewer::FRAME,
    //                    QGLViewer::ROTATE);
    //    setMouseBinding(Qt::NoModifier, Qt::RightButton, QGLViewer::FRAME,
    //                    QGLViewer::TRANSLATE);
    //    setMouseBinding(Qt::NoModifier, Qt::MidButton, QGLViewer::FRAME,
    //                    QGLViewer::ZOOM);
    //    setWheelBinding(Qt::NoModifier, QGLViewer::FRAME, QGLViewer::ZOOM);

    //    setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, QGLViewer::FRAME,
    //                    QGLViewer::ROTATE, false);
    //    setMouseBinding(Qt::ShiftModifier, Qt::RightButton, QGLViewer::FRAME,
    //                    QGLViewer::TRANSLATE, false);
    //    setMouseBinding(Qt::ShiftModifier, Qt::MidButton, QGLViewer::FRAME,
    //                    QGLViewer::ZOOM, false);
    //    setWheelBinding(Qt::ShiftModifier, QGLViewer::FRAME, QGLViewer::ZOOM, false);


    //    setKeyDescription(Qt::Key_G, "Change translation constraint direction");
    //    setKeyDescription(Qt::Key_D, "Change rotation constraint direction");
    //    setKeyDescription(Qt::Key_Space, "Change constraint reference");
    //    setKeyDescription(Qt::Key_T, "Change translation constraint type");
    //    setKeyDescription(Qt::Key_R, "Change rotation constraint type");
    //GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


    GLfloat light_ambient[]={0.2,0.2,0.2,1.0};
    GLfloat light_diffuse[]={1.0,1.0,1.0,1.0};
    GLfloat light_specular[]={1.0,1.0,1.0,1.0};
    GLfloat light_position[]={2.0,2.0,2.0,0.0};
    GLfloat mat_diffuse[]={0.8,0.8,0.8,1.0};

    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_CULL_FACE);
    //启用双面光照
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);/*材质测散射颜色*/





    restoreStateFromFile();

    qglviewer::Camera *cam = this->camera();
    cam->setType(qglviewer::Camera::ORTHOGRAPHIC);
    //cam->setSceneRadius(100);
    // connectAllCameraKFIInterpolatedSignals(false);
    //    qglviewer::Vec v1(-1000,-1000,-1000);
    //    qglviewer::Vec v2(1000,1000,1000);
    //    cam->setSceneBoundingBox(v1,v2);

}

void BasicViewer::initializeGL()
{
    QGLViewer::initializeGL();
    camera()->setZNearCoefficient(0.000001);
    camera()->setZClippingCoefficient(100000.0);
}

void BasicViewer::drawmesh()
{
    for (int j =0;j<m_vmesh.size();j++)
    {
        for (int i = 0; i < m_vmesh[j]->face_number; i++)
        {
            //注意我们的操作都是
            Face& face = m_vmesh[j]->faces[i];
            glBegin(GL_POLYGON);              //绘制多边形即可！

            glColor3f(m_vmesh[j]->mesh_color[0], m_vmesh[j]->mesh_color[1], m_vmesh[j]->mesh_color[2]);
            //在绘制面的过程中载入我们已经计算好的法线量信息
            glNormal3fv(face.normal);           //在绘制面的时候同时载入法向量信息
            for (int j = 0; j < face.vert_number; j++)
            {
                Vertex *vert = face.verts[j];
                glVertex3f(vert->x, vert->y, vert->z);
            }
            glEnd();
        }
    }
}

void BasicViewer::drawLine()
{

    float m_nclolr = 1/ (float)m_vline.size();
    for (int i =0 ;i<m_vline.size();i++)
    {
        double temp_x,temp_y,temp_z;
        glBegin(GL_LINES);
        //glColor3f(m_vline[i]->line_color[0], m_vline[i]->line_color[1], m_vline[i]->line_color[2]);

        //glColor3f(1,0,0);
        float cn = 1/float(m_vline[i]->line_number);
        double c_x = 0,c_y = 0,c_z = 0;
        for (int j = 0; j < m_vline[i]->line_number; j++)
        {
            Vertex vert = m_vline[i]->point[j];
            c_x+=vert.x;
            c_y+=vert.y;
            c_z+=vert.z;
            if(j==0)
            {
                temp_x  = vert.x;
                temp_y  = vert.y;
                temp_z  = vert.z;
                continue;
            }
            glColor3f(j*cn,1-j*cn,1-j*cn);
            //cout<<j*cn<<endl;
            glVertex3f(temp_x, temp_y, temp_z);
            glVertex3f(vert.x, vert.y, vert.z);
            temp_x  = vert.x;
            temp_y  = vert.y;
            temp_z  = vert.z;


        }

        glEnd();

        c_x = c_x/double(m_vline[i]->line_number);
        c_y = c_y/double(m_vline[i]->line_number);
        c_z = c_z/double(m_vline[i]->line_number);

        //m_vcenter.setValue(c_x,c_y,c_z);
    }


}

void BasicViewer::drawpoint()
{
    for (int i =0;i<m_vpoint.size();i++)
    {
        //下面绘制点的信息
        //依次将面的信息点进行遍历

        glColor3f(m_vpoint[i]->line_color[0], m_vpoint[i]->line_color[1], m_vpoint[i]->line_color[2]);
        glPointSize(2);
        glBegin(GL_POINTS);
        for (int j  = 0 ;  j < m_vpoint[i]->point_number; j++) {
            glVertex3f(m_vpoint[i]->point[j].x, m_vpoint[i]->point[j].y,m_vpoint[i]->point[j].z);
        }
        glEnd();
    }

}


#define PI 3.1415926535
#include <math.h>
void BasicViewer::drawarc()
{
    /*1*/
    double R = 10;
    double firstpoint[3] = {0,0,0};
    double endpoint[3] = {0,10,0};


    /*计算圆心坐标*/
    double arcpoint[3];

    arcpoint[0] = (firstpoint[0]+endpoint[0])/2.0;
    arcpoint[1] = (firstpoint[1]+endpoint[1])/2.0;
    arcpoint[2] = (firstpoint[2]+endpoint[2])/2.0;

    /*如果Y相同*/
    if(firstpoint[1] == endpoint[1])
    {
        double l1 = sqrt((firstpoint[0]-endpoint[0])*(firstpoint[0]-endpoint[0])
                + (firstpoint[1]-endpoint[1])*(firstpoint[1]-endpoint[1]));
        double l2 = sqrt(R*R-(l1/2.0)*(l1/2.0));
        arcpoint[0] = arcpoint[0] + l2;
    }
    /*如果X相同*/
    else if(firstpoint[0]==endpoint[0])
    {
        double l1 = sqrt((firstpoint[0]-endpoint[0])*(firstpoint[0]-endpoint[0])
                + (firstpoint[1]-endpoint[1])*(firstpoint[1]-endpoint[1]));
        double l2 = sqrt(R*R-(l1/2.0)*(l1/2.0));
        arcpoint[1] = arcpoint[1] + l2;
    }
    else
    {
        /*计算斜率*/
        double k = (firstpoint[1]-endpoint[1])/(firstpoint[0]-endpoint[0]);
        double l1 = sqrt((firstpoint[0]-endpoint[0])*(firstpoint[0]-endpoint[0])
                + (firstpoint[1]-endpoint[1])*(firstpoint[1]-endpoint[1]));
        double l2 = sqrt(R*R-(l1/2.0)*(l1/2.0));
        double at = atan(-1/k)*180/PI;
        arcpoint[0] = l2 * sin(at)+arcpoint[0];
        arcpoint[1] = l2 * cos(at)+arcpoint[1];
        arcpoint[2] = arcpoint[2];
    }
}

void BasicViewer::setScene(Scene *pScene)
{

}

void BasicViewer::mousePressEvent(QMouseEvent *e)
{
    QGLViewer::mousePressEvent(e);
}

void BasicViewer::mouseReleaseEvent(QMouseEvent *e)
{
    QGLViewer::mouseReleaseEvent(e);
}

void BasicViewer::drawAxisxyz(qglviewer::Vec v)
{
    glLineWidth(2.0);
    glBegin(GL_LINES);
    float size = 10;

    double fx=0,fy=0,fz=0;
    fx = v[0];
    fy = v[1];
    fz = v[2];
    // # draw base x-axis lines
    glColor3f(1, 0, 0);
    glVertex3d(0+fx, 0+fy, 0+fz);
    glVertex3d(size+fx, 0+fy, 0+fz);
    // # draw 'X'
    glVertex3d(1.05 * size+fx, 0.1 * size+fy, 0+fz);
    glVertex3d(1.2 * size+fx, -0.1 * size+fy, 0+fz);
    glVertex3d(1.05 * size+fx, -0.1 * size+fy, 0+fz);
    glVertex3d(1.2 * size+fx, 0.1 * size+fy, 0+fz);

    // # draw base y_axis-lines
    glColor3f(0, 1, 0);
    glVertex3d(0+fx, 0+fy, 0+fz);
    glVertex3d(0+fx, size+fy, 0+fz);
    // # draw 'Y'
    glVertex3d(0.075 * size+fx, 1.3 * size+fy, 0+fz);
    glVertex3d(0+fx, 1.15 * size+fy, 0+fz);
    glVertex3d(-0.075 * size+fx, 1.3 * size+fy, 0+fz);
    glVertex3d(0+fx, 1.15 * size+fy, 0+fz);
    glVertex3d(0+fx, 1.05 * size+fy, 0+fz);
    glVertex3d(0+fx, 1.15 * size+fy, 0+fz);

    // # draw base z_axis-lines
    glColor3f(0, 0, 1);
    glVertex3d(0+fx, 0+fy, 0+fz);
    glVertex3d(0+fx, 0+fy, size+fz);
    //   # draw 'Z'
    glVertex3d(-0.075*size+fx,      0.1*size+fy, fz+1.05*size);
    glVertex3d(0.075*size+fx,       0.1*size+fy, fz+1.05*size);
    glVertex3d(-0.075 * size+fx, -0.1 * size+fy, fz+1.05 * size);
    glVertex3d(0.075 * size+fx,  -0.1 * size+fy, fz+1.05 * size);
    glVertex3d(0.075 * size+fx,   0.1 * size+fy, fz+1.05 * size);
    glVertex3d(-0.075 * size+fx, -0.1 * size+fy, fz+1.05 * size);

    glEnd();
    glLineWidth(1.0);
    glEndList();

}
