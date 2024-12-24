QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CCOpenGL
TEMPLATE = app

win32{

LIBS += -lopengl32 -lglu32

}

SOURCES += \
        main.cpp \
        CCOpenGLWidget.cpp

HEADERS += \
        CCOpenGLWidget.h
