#include "mapdesigner.h"

MapDesigner::MapDesigner(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Display()));
}

MapDesigner::~MapDesigner()
{

}

void MapDesigner::Display()
{
    other = new MapDesignerWindow();
    other->setWindowModality(Qt::ApplicationModal);
    other->show();
}
