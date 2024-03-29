#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T12:26:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_16_17
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainview.cpp \
    model.cpp \
    utility.cpp \
    user_input.cpp \
    cat.cpp

HEADERS  += mainwindow.h \
    mainview.h \
    model.h \
    vertex.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
