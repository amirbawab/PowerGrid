#include "mapdesigner.h"
#include "MapDesignerWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapDesigner w;
//    MapDesignerWindow w;
    w.show();
    return a.exec();
}
