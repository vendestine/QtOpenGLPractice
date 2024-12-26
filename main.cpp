#include "QtFrameBuffer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFrameBuffer w;
    w.setFixedSize(800,600);
    w.show();

    return a.exec();
}
