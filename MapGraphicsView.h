#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include "City.h"
#include <memory>
#include <map>
#include "ConnectionItem.h"
#include "CityItem.h"

class MapGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    MapGraphicsView();
    ~MapGraphicsView() {}
    void Refresh();
    void DrawMap();

    // TODO: remove this
//    bool GetSelectCity() const { return selectCity; }
//    void SetSelectCity(bool selectCity) { this->selectCity = selectCity; }

private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    int scaleSteps = 0;
    const int MAX_ZOOM = 10;
    const int MIN_ZOOM = -10;
    float scaleFactor = 1.15f;
    std::map<string, shared_ptr<CityItem>> citiesItemsMap;
    vector<std::unique_ptr<ConnectionItem>> connectionItems;
    
    // TODO: remove this
//    // Used when we want to select a city from the map
//    bool selectCity = false;

    std::shared_ptr<City> GetCityByPoint(QPoint point);

    void MarkCitiesToRemove(std::shared_ptr<City> city);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void CitySelected(std::shared_ptr<City> city);
};

