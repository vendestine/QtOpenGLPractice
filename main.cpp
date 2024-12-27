#include "CCOpenGLWidget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CCOpenGLWidget w;
    w.show();

//    qDebug() << "\n";
//    qDebug() << "OpenGl information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
//    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
//    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
//    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
//    qDebug() << "\n";


    return a.exec();
}
