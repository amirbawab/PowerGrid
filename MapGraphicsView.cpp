#include "MapGraphicsView.h"

MapGraphicsView::MapGraphicsView() {
	
	// Set id
	setObjectName("mapGraphicsView");

	// Init components
	graphicsScene = new QGraphicsScene();

	// Set scene
	setScene(graphicsScene);

}

MapGraphicsView::~MapGraphicsView() {
	delete graphicsScene;
}

void MapGraphicsView::wheelEvent(QWheelEvent* event) {
	// Don't zoom if nothing is in the scene
	if (scene()->items().size() == 0)
		return;

	setTransformationAnchor(AnchorUnderMouse);
	setDragMode(ScrollHandDrag);

	if (event->delta() > 0) {
		scaleSteps++;
		scale(scaleFactor, scaleFactor);
	}
	else {
		scaleSteps--;
		scale(1 / scaleFactor, 1 / scaleFactor);
	}
}

void MapGraphicsView::Refresh() {
    
    std::map<std::string, std::shared_ptr<City>> citiesMap = DataStore::getInstance().map->GetCities();
    std::vector<std::shared_ptr<Connection>> connections = DataStore::getInstance().map->GetConnections();

    // Loop on all cities
    for (auto city : citiesMap) {
        
        graphicsScene.release();
        graphicsScene = std::make_unique<QGraphicsScene>();
        setScene(graphicsScene.get());

        for (auto& connection : connections) {
            auto costTextItem = connection->GetCostTextItem(connectionFont);
            auto costEllipseItem = connection->GetCostEllipseItem(connectionFont);

            scene()->addItem(connection.get());
            scene()->addItem(costEllipseItem);
            scene()->addItem(costTextItem);
        }

        for (auto cityMapItem : cities) {
            auto cityNameTextItem = cityMapItem.second->GetNameTextItem(cityFont);
            scene()->addItem(cityNameTextItem);
            scene()->addItem(cityMapItem.second.get());

            for (int i = 0; i < cityMapItem.second->GetHouses().size(); i++) {
                auto houseLocation = cityMapItem.second->GetHousePosition(i);
                auto houseItem = cityMapItem.second->GetHouses()[i];
                houseItem->setRect(houseLocation.x() - houseItem->rect().width() / 2,
                    houseLocation.y() - houseItem->rect().height() / 2,
                    houseItem->rect().width(), houseItem->rect().height());
                scene()->addItem(houseItem.get());
            }
        }

    }
}