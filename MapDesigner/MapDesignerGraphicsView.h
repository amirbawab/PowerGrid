#pragma once

#include <QGraphicsView>
#include <Connection.h>
#include "pugixml.hpp"

class MapDesignerGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    const QColor DEFAULT_REGION_COLOR = Qt::white;

    MapDesignerGraphicsView();

    bool SetRegionColor(QColor regionColor);
    std::vector<QColor>& GetRegionColors() { return regionColors; }
    std::map<std::string, std::shared_ptr<City>>& GetCities() { return cities; }
    std::vector<std::unique_ptr<Connection>>& GetConnections() { return connections; }

private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    const int CITY_WIDTH  = 70;
    const int CITY_HEIGHT = 70;
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
    void PopulateCities(pugi::xml_node& map);
    void PopulateConnections(pugi::xml_node& map);

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
    void OnExportXml();
};

