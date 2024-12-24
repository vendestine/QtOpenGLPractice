#-------------------------------------------------
#
# Project created by QtCreator 2020-10-11T13:45:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGL
TEMPLATE = app

win32{

DEFINES += CCOPENGL_PLATFORM_WIN32
LIBS += -lOpengl32 -lglu32

}

macx{

DEFINES +=  CCOPENGL_PLATFORM_MACX
QMAKE_LFLAGS += -framework OpenGL

}


SOURCES += \
        main.cpp \
        CCOpenGLWidget.cpp

HEADERS += \
        CCOpenGLWidget.h
