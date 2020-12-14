#include "gcodeinterpreter.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>

GCodeInterpreter::GCodeInterpreter(const QString &content)
    : m_gcodeContent(content)
{
    m_sfilename = content.toStdString();

    m_listdata = getstrsplit(m_sfilename);
}

QList<GCodeCommand> GCodeInterpreter::interpret() const
{

    QList<GCodeCommand> result;

    string commandStr;
    bool xyzUsed[3] = {false, false, false};
    string lastCode = "G00";

    GCodeCommand *lastcode= NULL;
    foreach (const string &s, m_listdata) {
        auto firstChar = s[0];

        if (firstChar == 'G')
        {
            lastCode = s;

            if (commandStr.size()) {
                GCodeCommand gcode(QString::fromStdString(commandStr),lastcode);
                result << gcode;
                lastcode = &gcode;
                //cout<<commandStr<<endl;
                commandStr.clear();
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }

        } else if (firstChar == QChar('X')) {

            if (xyzUsed[0]) {
                GCodeCommand gcode(QString::fromStdString(commandStr),lastcode);
                result << gcode;
                lastcode = &gcode;
                //cout<<commandStr<<endl;
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[0] = true;

        } else if (firstChar == QChar('Y')) {

            if (xyzUsed[1]) {
                GCodeCommand gcode(QString::fromStdString(commandStr),lastcode);
                result << gcode;
                lastcode = &gcode;
                //cout<<commandStr<<endl;
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[1] = true;

        } else if (firstChar == QChar('Z')) {

            if (xyzUsed[2]) {
                GCodeCommand gcode(QString::fromStdString(commandStr),lastcode);
                result << gcode;
                lastcode = &gcode;
                //cout<<commandStr<<endl;
                commandStr.clear();
                commandStr.append(lastCode);
                xyzUsed[0] = xyzUsed[1] = xyzUsed[2] = false;
            }
            xyzUsed[2] = true;
        }
        commandStr.append(s);
    }
    GCodeCommand gcode(QString::fromStdString(commandStr),lastcode);
    result << gcode;
    lastcode = &gcode;
    //cout<<commandStr<<endl;

    return result;
}

vector<string> GCodeInterpreter::getstrsplit(string str) const
{
    fstream infile;
    vector<string> relist;
    infile.open(str.c_str(),ios::in);
    if(infile.is_open())
    {

        char buffer[1024];
        while (!infile.eof())
        {
            infile.getline(buffer ,1024);
            int bn = 0;
            string strbmp = string(buffer);

            int nstart = 0,nstop = 0;

            bool bs = false;
            while (1)
            {
                /*GXYZFRIJK*/
                if(buffer[bn] == 'G'||
                        buffer[bn] == 'X'||
                        buffer[bn] == 'Y'||
                        buffer[bn] == 'Z'||
                        buffer[bn] == 'F'||
                        buffer[bn] == 'R'||
                        buffer[bn] == 'I'||
                        buffer[bn] == 'J'||
                        buffer[bn] == 'K'||
                        buffer[bn] == 'N'
                        )
                {
                    if(!bs)
                    {
                        bs = true;
                        nstop = 0;
                    }
                    else
                    {

                        relist.push_back(strbmp.substr(nstart,nstop));
                        nstart = bn;
                        nstop = 0;

                    }
                }
                if(buffer[bn] =='\n' ||
                        buffer[bn] =='\0'||
                        buffer[bn] =='H'||
                        buffer[bn] =='P'||
                        buffer[bn] =='M'||
                        buffer[bn] =='S'
                        )
                {

                    relist.push_back(strbmp.substr(nstart,nstop));
                    nstart = nstop;
                    break;
                }
                nstop++;
                bn++;
            }
        }
    }
    return  relist;
}







































