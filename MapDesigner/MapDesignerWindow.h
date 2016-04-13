#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include "MapDesignerGraphicsView.h"

class MapDesignerWindow : public QMainWindow
{
    Q_OBJECT

    QWidget                 * centralWidget;
    QGridLayout             * layout;
    MapDesignerGraphicsView * graphicsView;
    QPushButton             * addCityButton;
    QPushButton             * deleteCityButton;
    QPushButton             * changeRegionColorButton;
    QPushButton             * addConnectionButton;
    QPushButton             * exportXml;
    QPushButton             * loadXml;
    QStatusBar              * statusBar;
    QLabel                  * regionColor;
    QLabel                  * regionColorLabel;
    QHBoxLayout             * hLayout;

    const int WIDTH  = 1700;
    const int HEIGHT = 900;

public:
    MapDesignerWindow();
    ~MapDesignerWindow();

private slots:
    void OnDisplayMessage(QString message) const;
    void OnClearMessage() const;
    void OnChangeRegionColor() const;
};
