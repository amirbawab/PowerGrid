#ifndef MAPDESIGNER_H
#define MAPDESIGNER_H

#include <QtWidgets/QMainWindow>
#include "ui_mapdesigner.h"
#include "MapDesignerWindow.h"

class MapDesigner : public QMainWindow
{
    Q_OBJECT

    MapDesignerWindow * other;

public:
    MapDesigner(QWidget *parent = 0);
    ~MapDesigner();

private:
    Ui::MapDesignerClass ui;

private slots:
    void Display();
};

#endif // MAPDESIGNER_H
