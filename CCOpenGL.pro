#-------------------------------------------------
#
# Project created by QtCreator 2020-10-20T20:45:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGL
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

RESOURCES += \
    ccopengl.qrc
