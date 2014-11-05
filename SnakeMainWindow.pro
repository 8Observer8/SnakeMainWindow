#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T14:06:27
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SnakeMainWindow
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Scene.cpp

HEADERS  += MainWindow.h \
    Scene.h

FORMS    += MainWindow.ui
