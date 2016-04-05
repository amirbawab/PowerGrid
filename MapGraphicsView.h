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
public:
	MapGraphicsView();
    ~MapGraphicsView() {}
    void Refresh();
    void DrawMap();
private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    int scaleSteps = 0;
	float scaleFactor = 1.15f;
    std::map<string, std::shared_ptr<CityItem>> citiesItemsMap;
    std::vector<std::unique_ptr<ConnectionItem>> connectionItems;
protected:
	void wheelEvent(QWheelEvent* event) override;
};

