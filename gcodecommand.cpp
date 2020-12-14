#include "gcodecommand.h"
#include <QRegExp>
#include <qdebug.h>
GCodeCommand::GCodeCommand(const QString &rawStr, GCodeCommand *lastcode)
    : m_code(G00),
      m_rawString(rawStr), m_fillRate(100), m_raduis(0),
      m_overrideFeedRate(false)
{
    for (auto &i : m_usedCoord) {
        i = false;
    }
    m_plastcode = lastcode;

    parse();
}


void GCodeCommand::parse()
{
    int loc[10];

    int numtmp = 0;

    bool nxyz[3] = {false,false,false};
    int nG=-1,nI=-1,nX=-1,nJ=-1,nY=-1,nK=-1,nZ=-1,nF=-1,nR=-1,nA = -1,nC = -1,nB = -1;
    for(int i =0;i<m_rawString.size();i++)
    {
        switch (m_rawString[i].toLatin1())
        {
        case 'G':
            loc[numtmp] = i;
            nG = numtmp;
            numtmp++;
            break;
        case 'I':
            loc[numtmp] = i;
            nI = numtmp;
            numtmp++;
            break;
        case 'X':
            loc[numtmp] = i;
            nX = numtmp;
            numtmp++;
            break;
        case 'J':
            loc[numtmp] = i;
            nJ = numtmp;
            numtmp++;
            break;
        case 'Y':
            loc[numtmp] = i;
            nY = numtmp;
            numtmp++;
            break;
        case 'K':
            loc[numtmp] = i;
            nK = numtmp;
            numtmp++;
            break;
        case 'Z':
            loc[numtmp] = i;
            nZ= numtmp;
            numtmp++;
            break;
        case 'F':
            loc[numtmp] = i;
            nF = numtmp;
            numtmp++;
            break;
        case 'R':
            loc[numtmp] = i;
            nR = numtmp;
            numtmp++;
            break;
        case 'N':
            loc[numtmp] = i;
            nR = numtmp;
            numtmp++;
            break;
        case 'A':
            loc[numtmp] = i;
            nA = numtmp;
            numtmp++;
            break;
        case 'C':
            loc[numtmp] = i;
            nC = numtmp;
            numtmp++;
            break;
        case 'B':
            loc[numtmp] = i;
            nB = numtmp;
            numtmp++;
            break;
        default:
            break;
        }
    }

    loc[numtmp] = m_rawString.size();


    if(nG != -1)
    {
        bool bb = false;
        int nncode;
        nncode = m_rawString.mid(loc[nG]+1,loc[nG+1]-loc[nG]-1).toInt(&bb);
       // qDebug()<<nncode;
        if(bb == true)
        {
            m_code =static_cast<CodeType>(nncode);
        }
        else
        {
            m_code = CodeType::G00;
        }
    }

    if(nI != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nI]+1,loc[nI+1]-loc[nI]-1).toFloat(&bb);
        if(bb == true)
        {
            m_center.setX(nncode);
            m_usedCoord[0] = true;
        }

    }
    if(nX != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nX]+1,loc[nX+1]-loc[nX]-1).toFloat(&bb);
        if(bb == true)
        {
            m_vector.setX(nncode);
            m_usedCoord[0] = true;
            nxyz[0] = true;
        }

    }

    if(nJ != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nJ]+1,loc[nJ+1]-loc[nJ]-1).toFloat(&bb);
        if(bb == true)
        {
            m_center.setY(nncode);
            m_usedCoord[1] = true;
        }

    }
    if(nY != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nY]+1,loc[nY+1]-loc[nY]-1).toFloat(&bb);
        if(bb == true)
        {
            m_vector.setY(nncode);
            m_usedCoord[1] = true;
            nxyz[1] = true;
        }

    }
    if(nK != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nK]+1,loc[nK+1]-loc[nK]-1).toFloat(&bb);
        if(bb == true)
        {
            m_center.setZ(nncode);
            m_usedCoord[2] = true;
        }

    }
    if(nZ != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nZ]+1,loc[nZ+1]-loc[nZ]-1).toFloat(&bb);
        if(bb == true)
        {
            m_vector.setZ(nncode);
            m_usedCoord[2] = true;
            nxyz[2] = true;
        }

    }
    if(nF != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nF]+1,loc[nF+1]-loc[nF]-1).toFloat(&bb);
        if(bb == true)
        {
            m_fillRate = nncode;
            m_overrideFeedRate = true;
        }

    }

    if(nR != -1)
    {
        bool bb = false;
        float nncode;
        nncode = m_rawString.mid(loc[nR]+1,loc[nR+1]-loc[nR]-1).toFloat(&bb);
        if(bb == true)
        {
            m_raduis = nncode;
        }

    }




    if(m_plastcode != NULL)
    {

        if(nxyz[0] == false)
        {
            m_vector.setX(m_plastcode->m_vector.x());
        }
        if(nxyz[1] == false)
        {
            m_vector.setY(m_plastcode->m_vector.y());
        }
        if(nxyz[2] == false)
        {
            m_vector.setZ(m_plastcode->m_vector.z());
        }
    }
    QRegExp regExp("[GXYZFRIJK][-+]?[0-9]*\\.?[0-9]*", Qt::CaseInsensitive);

    //    int pos = 0;

    //    while ((pos = regExp.indexIn(m_rawString, pos)) != -1) {
    //        auto str = regExp.cap();
    //        switch (str.at(0).toUpper().toLatin1()) {

    //        case 'G':
    //            m_code = static_cast<CodeType>(QString(str.data() + 1).toUInt());
    //            break;
    //        case 'I':
    //            m_center.setX(QString(str.data() + 1).toDouble());
    //            m_usedCoord[0] = true;
    //            break;
    //        case 'X':
    //            m_vector.setX(QString(str.data() + 1).toDouble());
    //            m_usedCoord[0] = true;
    //            break;
    //        case 'J':
    //            m_center.setY(QString(str.data() + 1).toDouble());
    //            m_usedCoord[1] = true;
    //            break;
    //        case 'Y':
    //            m_vector.setY(QString(str.data() + 1).toDouble());
    //            m_usedCoord[1] = true;
    //            break;
    //        case 'K':
    //            m_center.setZ(QString(str.data() + 1).toDouble());
    //            m_usedCoord[2] = true;
    //            break;
    //        case 'Z':
    //            m_vector.setZ(QString(str.data() + 1).toDouble());
    //            m_usedCoord[2] = true;
    //            break;
    //        case 'F':
    //            m_fillRate = QString(str.data() + 1).toDouble();
    //            m_overrideFeedRate = true;
    //            break;
    //        case 'R':
    //            m_raduis = QString(str.data() + 1).toDouble();
    //            break;
    //        default:
    //            break;
    //        }
    //        pos += regExp.matchedLength();
    //    }
}

QVector3D GCodeCommand::mapToPlane(const QVector3D &pt,
                                   GCodeCommand::CodeType dstType) const
{
    if (dstType == G17) {
        return pt;
    }

    QVector3D dstPt;

    if (dstType == G18) {
        dstPt.setZ(pt.x());
        dstPt.setX(pt.y());
    } else if (dstType == G19) {
        dstPt.setY(pt.x());
        dstPt.setZ(pt.y());
    } else {
        Q_ASSERT(false);
    }

    return dstPt;
}
