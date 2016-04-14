#pragma once

#include <QGraphicsView>
#include "MapDesignerConnection.h"
#include "pugixml.hpp"
#include "set"

const std::string MAP_DESCRIPTION = "Created by PG MapDesigner";

class MapDesignerGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    const QColor DEFAULT_REGION_COLOR = Qt::white;

    MapDesignerGraphicsView();

    bool SetRegionColor(QColor regionColor);
    std::vector<QColor>& GetRegionColors() { return regionColors; }
    std::map<std::string, std::shared_ptr<MapDesignerCity>>& GetCities() { return cities; }
    std::vector<std::unique_ptr<MapDesignerConnection>>& GetConnections() { return connections; }

    void LoadXml(QString fileName);

private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    const int CITY_WIDTH  = 60;
    const int CITY_HEIGHT = 60;
    QColor regionColor = DEFAULT_REGION_COLOR;

    int scaleSteps = 0;
    float scaleFactor = 1.15f;

    bool addCity = false;
    bool deleteCity = false;
    bool addConnectionFirstCity = false;
    bool addConnectionSecondCity = false;

    std::unique_ptr<MapDesignerConnection> connection;
    std::shared_ptr<MapDesignerCity> connectionFirstCity;
    std::shared_ptr<MapDesignerCity> connectionSecondCity;

    std::vector<QColor> regionColors;
    std::set<QColor> loadedRegionColors;
    std::map<std::string, std::shared_ptr<MapDesignerCity>> cities;
    std::vector<std::unique_ptr<MapDesignerConnection>> connections;

    QFont cityFont;
    QFont connectionFont;

    void UpdateScene();
    std::string GetCityByPoint(QPoint point);
    void ResetScale();

    void PopulateCities(pugi::xml_node& map);
    void PopulateConnections(pugi::xml_node& map);

    bool LoadCities(pugi::xml_document& xml);
    bool LoadConnections(pugi::xml_document& xml);

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
    void OnDeleteCity();
    void OnAddConnection();
    void OnCancelOperation();
    void OnExportXml();
};
