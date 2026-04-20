#include <QtGui/QApplication>
#include "spaceinvaders.h"

// static
//int foo = bar;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpaceInvaders w;
    w.show();

    return a.exec();
}
