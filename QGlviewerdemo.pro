#-------------------------------------------------
#
# Project created by QtCreator 2020-11-18T11:06:52
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGlviewerdemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        basicviewer.cpp \
        gcodecommand.cpp \
        gcodeinterpreter.cpp \
        gcodevisualizer.cpp \
        main.cpp \
        modelfileread.cpp \
        widget.cpp

HEADERS += \
        basicviewer.h \
        gcodecommand.h \
        gcodeinterpreter.h \
        gcodevisualizer.h \
        modelfileread.h \
        viewertype.h \
        widget.h

FORMS += \
        widget.ui

#INCLUDEPATH += ../QGLViewer/

LIBS += -lopengl32 -lglu32
#LIBS += libQGLViewerd2

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QGLViewer/ -lQGLViewer2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QGLViewer/ -lQGLViewerd2

INCLUDEPATH += $$PWD/QGLViewer
DEPENDPATH += $$PWD/QGLViewer


