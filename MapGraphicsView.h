#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include "DataStore.h"
#include "City.h"
#include "Connection.h"
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

private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    int scaleSteps = 0;
    const int MAX_ZOOM = 10;
    const int MIN_ZOOM = -10;
    float scaleFactor = 1.15f;
    map<string, shared_ptr<CityItem>> citiesItemsMap;
    vector<std::unique_ptr<ConnectionItem>> connectionItems;
    
	std::shared_ptr<City> GetCityByPoint(QPoint point);

	void MarkCitiesToRemove(std::shared_ptr<City> city);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

