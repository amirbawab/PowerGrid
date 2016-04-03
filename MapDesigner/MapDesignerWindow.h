#pragma once

#include <QMainWindow>
#include <QtWidgets>

class MapDesignerWindow : public QMainWindow
{
    Q_OBJECT

    QWidget * centralWidget;
    QGridLayout * layout;
    QGraphicsView * graphicsView;
    QPushButton * addCityButton;
    QPushButton * changeRegionColorButton;
	QPushButton * addConnectionButton;
	QPushButton * exportXML;
    QStatusBar * statusBar;
    QLabel * regionColor;

    int mouseX = 0;
    int mouseY = 0;

public:
    MapDesignerWindow();
    ~MapDesignerWindow();

private slots:
    void OnDisplayMessage(QString message) const;
    void OnClearMessage() const;
    void OnChangeRegionColor() const;
	void OnExportXML() const;
};

