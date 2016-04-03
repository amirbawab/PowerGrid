#pragma once

#include <QGraphicsView>
#include <Connection.h>

class MapDesignerGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    const QColor DEFAULT_REGION_COLOR = Qt::white;

    MapDesignerGraphicsView();

    bool SetRegionColor(QColor regionColor);

private:
    const int CITY_WIDTH  = 30;
    const int CITY_HEIGHT = 30;
    QColor regionColor = DEFAULT_REGION_COLOR;

    int scaleSteps = 0;
    float scaleFactor = 1.15f;

    bool addCity = false;
    bool addConnectionFirstCity = false;
    bool addConnectionSecondCity = false;

    std::unique_ptr<Connection> connection;
    std::shared_ptr<City> connectionFirstCity;
    std::shared_ptr<City> connectionSecondCity;

    std::vector<QColor> regionColors;
    std::map<std::string, std::shared_ptr<City>> cities;
    std::vector<std::unique_ptr<Connection>> connections;

    QFont cityFont;
    QFont connectionFont;

    void UpdateScene();
    std::string GetCityByPoint(QPoint point);
    void ResetScale();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

signals:
    void DisplayMessage(QString message);
    void ClearMessage();

public slots:
    void OnAddCity();
    void OnAddConnection();
    void OnCancelOperation();
};

