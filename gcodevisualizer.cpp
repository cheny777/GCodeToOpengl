#include "gcodevisualizer.h"
#include <cmath>
#include <QVector2D>


namespace {
    QVector2D getCircleCenter(const QVector2D &p1,
                              const QVector2D &p2,
                              qreal r,
                              bool cw = true) {
        QVector3D middle = (p1 + p2) / 2;
        auto diff = p1 - p2;
        auto q = diff.length();

        QList<QVector2D> result;
        QVector2D center;
        auto p = sqrt(r * r - (q / 2) * (q / 2));
        center.setX(middle.x() + p * diff.y() / q);
        center.setY(middle.y() + p * (-diff.x()) / q);
        result.push_back(center);

        center.setX(middle.x() - p * diff.y() / q);
        center.setY(middle.y() - p * (-diff.x()) / q);
        result.push_back(center);

        if (r < 0) {
            cw = !cw;
        }

        if (cw) {
            return result[1];
        }

        return result[0];
    }
}

GCodeVisualizer::GCodeVisualizer()
{

}

QList<QVector3D> GCodeVisualizer::visualize(const QList<GCodeCommand> &list,
                                            QList<QPair<int, int> > &segmentInfo) const
{

    QList<QVector3D> result;
    QVector3D prevPt;
    CoordinateSystem coordSystem = Absolute;
    segmentInfo.clear();
    QPair<int, int> segInfoElement;

    foreach (auto &s, list) {
        segInfoElement.first = qMax<int>(result.size() - 1, 0);
        switch (s.code()) {
        case GCodeCommand::G00:
        case GCodeCommand::G01:
        case GCodeCommand::G02:
        case GCodeCommand::G03:
            /*
             * s:一行code
             * prevPt：点
             * result：点容器
             * coordSystem:类型
            */
            prevPt = visualizeAsMotionGCode(s, prevPt, result, coordSystem);
            break;
        case GCodeCommand::G90:/*绝对坐标*/
            coordSystem = Absolute;
            break;
        case GCodeCommand::G91:/*增量坐标*/
            coordSystem = Increment;
            break;
        default:
            break;
        }
        segInfoElement.second = qMax<int>(result.size() - 1, segInfoElement.first);
        segmentInfo.push_back(segInfoElement);
    }
    return result;
}



QVector3D GCodeVisualizer::visualizeAsMotionGCode(const GCodeCommand &s,
                                                  const QVector3D &prevPt,
                                                  QList<QVector3D> &result,
                                                  CoordinateSystem coordSystem) const
{
    auto curPt = s.vector();/*当前点*/

    if (coordSystem == Absolute)
    {
        if (!s.xChanged())
        {
            curPt.setX(prevPt.x());
        }

        if (!s.yChanged())
        {
            curPt.setY(prevPt.y());
        }

        if (!s.zChanged())
        {
            curPt.setZ(prevPt.z());
        }

    }
    else if (coordSystem == Increment)
    {
        curPt += prevPt;
    }

    if (s.code() <= GCodeCommand::G01)
    {
        result.push_back(curPt);
    } else if (s.code() == GCodeCommand::G02 ||
               s.code() == GCodeCommand::G03)
    {

        QVector2D center;
        auto p1 = prevPt.toVector2D();/*上一个点*/
        auto p2 = curPt.toVector2D();/*当前点*/

        if (s.radius())
        {
            center = getCircleCenter(p1,
                                     p2,
                                     s.radius(),
                                     s.code() == GCodeCommand::G02);
        } else
        {
            center = (prevPt + s.center()).toVector2D();
            auto d1 = (prevPt - center).length();
            auto d2 = (curPt - center).length();
            /*判断两点间距离必须相同*/
            Q_ASSERT(fabs(d1 - d2) <= 1e-2);
        }

        auto p = p1;


        qDebug()<<(p - p2).length();
        if((p - p2).length()<0.00001)
        {
            do {
                /*向量*/
                auto tangent = p - center;

                if (s.code() == GCodeCommand::G03)
                {
                    tangent *= -1;
                }
                /*标准向量单位*/
                tangent = 0.01 * QVector2D(tangent.y(), -tangent.x()).normalized();
                p += tangent;
                result.push_back(p);
            } while ((p - p2).length() < (p - center).length());

            do {
                /*向量*/
                auto tangent = p - center;

                if (s.code() == GCodeCommand::G03)
                {
                    tangent *= -1;
                }
                /*标准向量单位*/
                tangent = 0.01 * QVector2D(tangent.y(), -tangent.x()).normalized();
                p += tangent;
                QVector3D fv(p[0],p[1],prevPt[2]);
                result.push_back(fv);
            } while ((p - p2).length() > 0.05);


        }
        else
        {
            do {
                /*向量*/
                auto tangent = p - center;

                if (s.code() == GCodeCommand::G03)
                {
                    tangent *= -1;
                }
                /*标准向量单位*/
                tangent = 0.01 * QVector2D(tangent.y(), -tangent.x()).normalized();
                p += tangent;
                QVector3D fv(p[0],p[1],prevPt[2]);
                result.push_back(fv);
            } while ((p - p2).length() > 0.05);
        }

        result.push_back(curPt);
    }
    return curPt;
}
