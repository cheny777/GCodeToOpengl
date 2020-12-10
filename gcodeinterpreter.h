#ifndef GCODEINTERPRETER_H
#define GCODEINTERPRETER_H

#include <QList>
#include <QString>
#include "gcodecommand.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <QtGlobal>
#include <QStringList>
using namespace std;

class GCodeInterpreter
{
public:
    explicit GCodeInterpreter(const QString &content);

public:
    QList<GCodeCommand> interpret() const;

private:
    QString m_gcodeContent;
    vector<string> getstrsplit(string str) const;

    string m_sfilename;
    vector<string> m_listdata;


};

#endif // GCODEINTERPRETER_H
