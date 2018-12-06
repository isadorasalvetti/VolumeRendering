
TEMPLATE = app
TARGET = Base

QT += gui opengl

CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    trianglemesh.h \
    rawreader.h \
    dialogresolution.h
FORMS += mainwindow.ui \
    dialogresolution.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    trianglemesh.cpp \
    rawreader.cpp \
    dialogresolution.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc
