#include "MapGraphicsView.h"

MapGraphicsView::MapGraphicsView() {
	
	// Set id
	setObjectName("mapGraphicsView");

	// Init components
	graphicsScene = std::make_unique<QGraphicsScene>();

	// Set scene
	setScene(graphicsScene.get());

    // Load cities and connections
    std::map<std::string, std::shared_ptr<City>> citiesMap = DataStore::getInstance().map->GetCities();
    std::vector<std::shared_ptr<Connection>> connections = DataStore::getInstance().map->GetConnections();

    // Create cities items
    for (auto city : citiesMap) {
        citiesItemsMap[city.first] = std::make_shared<CityItem>(QPoint(city.second->getX(), 
            city.second->getY()), city.second->getWidth(), city.second->getHeight());
        citiesItemsMap[city.first]->SetName(city.first);
        citiesItemsMap[city.first]->SetCity(city.second.get());
        citiesItemsMap[city.first]->SetRegionColor(QColor(city.second->GetRegion()->GetName().c_str()));
    }

    // Create connections items
    QFont connectionFont = QFont("Tahoma", 12, QFont::Bold);
    QFont cityFont = QFont("Calibri", 12, QFont::Bold, true);
    for (auto connection : connections) {
        connectionItems.push_back(std::make_unique<ConnectionItem>());

        // Set cities
        auto connectionItem = connectionItems[connectionItems.size() - 1].get();
        connectionItem->SetFirstCity(citiesItemsMap[connection->GetFirst()->GetName()]);
        connectionItem->SetSecondCity(citiesItemsMap[connection->GetSecond()->GetName()]);

        // Set connection
        connectionItem->SetConnection(connection);

        // Style
        auto costTextItem = connectionItem->GetCostTextItem(connectionFont);
        auto costEllipseItem = connectionItem->GetCostEllipseItem(connectionFont);

        // Adding components
        scene()->addItem(connectionItem);
        scene()->addItem(costEllipseItem);
        scene()->addItem(costTextItem);
    }

    // Ading cities
    for (auto city : citiesMap) {
        auto cityNameTextItem = citiesItemsMap[city.first]->GetNameTextItem(cityFont);
        scene()->addItem(cityNameTextItem);
        scene()->addItem(citiesItemsMap[city.first].get());
    }
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
    
    

   
}