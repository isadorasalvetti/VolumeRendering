
TEMPLATE = app
TARGET = Base

QT += gui opengl

CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    rawreader.h \
    dialogresolution.h \
    volumerender.h
FORMS += mainwindow.ui \
    dialogresolution.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    dialogresolution.cpp \
    volumerender.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
