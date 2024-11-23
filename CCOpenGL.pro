QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGL
TEMPLATE = app

win32{
# 链接 OpenGL 和 GLU 库
LIBS += -lopengl32 -lglu32

}



SOURCES += \
        main.cpp \
        CCOpenGLWidget.cpp

HEADERS += \
        CCOpenGLWidget.h
