#ifndef GCODEVISUALIZER_H
#define GCODEVISUALIZER_H

#include <QVector3D>
#include <QList>
#include <QPair>
#include "gcodecommand.h"
#include <qdebug.h>

class GCodeVisualizer
{
public:
    GCodeVisualizer();

public:
    QList<QVector3D> visualize(const QList<GCodeCommand> &list,
                               QList<QPair<int, int> > &segmentInfo) const;

private:
    enum CoordinateSystem {
        Increment = 0,/*增量坐标*/
        Absolute,/*绝对坐标*/
    };

    QVector3D visualizeAsMotionGCode(const GCodeCommand &s,
                                     const QVector3D &prevPt,
                                     QList<QVector3D> &result,
                                     CoordinateSystem coordSystem = Absolute) const;
};

#endif // GCODEVISUALIZER_H
