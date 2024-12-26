#-------------------------------------------------
#
# Project created by QtCreator 2020-04-07T19:21:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFrameBuff
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

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
        QtFrameBuffer.cpp

HEADERS += \
        QtFrameBuffer.h

RESOURCES += \
    qframebuffer.qrc
