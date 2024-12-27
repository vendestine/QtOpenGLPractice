#-------------------------------------------------
#
# Project created by QtCreator 2020-04-10T22:23:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGLShader
TEMPLATE = app


macx{

DEFINES += CCOPENGL_PLATFORM_MACX
QMAKE_LFLAGS += -framework OpenGL

}

win32{

DEFINES += CCOPENGL_PLATFORM_WIN32
LIBS += -lopengl32 -lglu32

}

SOURCES += \
        main.cpp \
        CCOpenGLWidget.cpp

HEADERS += \
        CCOpenGLWidget.h
