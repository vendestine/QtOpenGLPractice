#include "CCOpenGLWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCOpenGLWidget w;
    w.show();

    return a.exec();
}
