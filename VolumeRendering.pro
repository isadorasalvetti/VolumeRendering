
TEMPLATE = app
TARGET = Base

QT += gui opengl

CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += glwidget.h mainwindow.h \
    rawreader.h \
    dialogresolution.h \
    volume.h \
    viewplane.h
FORMS += mainwindow.ui \
    dialogresolution.ui
SOURCES += glwidget.cpp main.cpp mainwindow.cpp \
    rawreader.cpp \
    dialogresolution.cpp \
    volume.cpp \
    viewplane.cpp

DISTFILES += \
    shaders/simpleshader.vert \
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc