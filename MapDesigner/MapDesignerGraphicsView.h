#pragma once

#include <QGraphicsView>
#include <Connection.h>

class MapDesignerGraphicsView : public QGraphicsView
{
    Q_OBJECT

    const int CITY_WIDTH  = 30;
    const int CITY_HEIGHT = 30;
    QColor regionColor = Qt::white;

    bool addCity = false;
    bool addConnectionFirstCity = false;
    bool addConnectionSecondCity = false;

    std::unique_ptr<Connection> connection;
    std::shared_ptr<City> connectionFirstCity;
    std::shared_ptr<City> connectionSecondCity;

//    std::vector<std::shared_ptr<City>> cities;
    std::map<std::string, std::shared_ptr<City>> cities;
    std::vector<std::unique_ptr<Connection>> connections;

    QFont cityFont;
    QFont connectionFont;

private:
    void UpdateScene();
    std::string GetCityByPoint(QPoint point);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

public:
    MapDesignerGraphicsView();

    void SetRegionColor(QColor regionColor) { this->regionColor = regionColor; }

signals:
    void DisplayMessage(QString message);
    void ClearMessage();

public slots:
    void OnAddCity();
    void OnAddConnection();
    void OnCancelOperation();
};

