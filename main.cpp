#include "canvas.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Canvas w;
    w.setMouseTracking(true);
    w.show();
    return a.exec();
}
