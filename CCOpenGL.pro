#-------------------------------------------------
#
# Project created by QtCreator 2020-10-11T13:45:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGL
TEMPLATE = app

macx{

DEFINES += CCOPENGL_PLATFORM_MACX
QMAKE_LFLAGS += -framework OpenGL

INCLUDEPATH += $$PWD/3rdparty/mac/libglew/include
LIBS += -L$$PWD/3rdparty/mac/libglew/lib -lGLEW

}

win32{

DEFINES += CCOPENGL_PLATFORM_WIN32
LIBS += -lopengl32 -lglu32

INCLUDEPATH += $$PWD/3rdparty/win32/libglew/include
LIBS += $$PWD/3rdparty/win32/libglew/lib/glew32.lib

}



SOURCES += \
        main.cpp \
        CCOpenGLWidget.cpp

HEADERS += \
        CCOpenGLWidget.h

RESOURCES += \
    cc3d.qrc
