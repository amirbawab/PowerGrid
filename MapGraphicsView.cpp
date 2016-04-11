#include "MapGraphicsView.h"
#include "Game.h"

MapGraphicsView::MapGraphicsView() {
    
    // Set id
    setObjectName("mapGraphicsView");

    // Init components
    graphicsScene = std::make_unique<QGraphicsScene>();

    // Set scene
    setScene(graphicsScene.get());

    // Configure scence
    setRenderHints(QPainter::Antialiasing
        | QPainter::SmoothPixmapTransform
        | QPainter::TextAntialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
}

shared_ptr<City> MapGraphicsView::GetCityByPoint(QPoint point)
{
    for (auto cityItem : citiesItemsMap)
        if (cityItem.second->contains(point))
            return cityItem.second->GetCity();

    return nullptr;
}

void MapGraphicsView::MarkCitiesToRemove(shared_ptr<City> city)
{
    for (auto cityItem : citiesItemsMap) {
        // TODO mark cities toRemove
    }
}

void MapGraphicsView::wheelEvent(QWheelEvent* event) {
    // Don't zoom if nothing is in the scene
    if (scene()->items().size() == 0)
        return;

    if (event->delta() > 0 && scaleSteps < MAX_ZOOM) {
        scaleSteps++;
        scale(scaleFactor, scaleFactor);
    }
    else if (event->delta() < 0 && scaleSteps > MIN_ZOOM) {
        scaleSteps--;
        scale(1 / scaleFactor, 1 / scaleFactor);
    }
}

void MapGraphicsView::mousePressEvent(QMouseEvent* event)
{
    // Super
    QGraphicsView::mousePressEvent(event);

    // Only process left clicks
    if (event->button() != Qt::LeftButton || !Game::getInstance().selectCity)
        return;

    // 'mapToScene' returns QPointF; convert to QPoint
    auto scenePoint = mapToScene(event->pos());
    auto point = QPoint(scenePoint.x(), scenePoint.y());

    // If no city contains this point
    auto city = GetCityByPoint(point);
    if (!city)
        return;

    emit CitySelected(city);

    // If starting game, select cities to remove
    if (Game::getInstance().GetPhase() == 0) {
        MarkCitiesToRemove(city);
    }
    else {
    
    }
}

void MapGraphicsView::Refresh() {
    
    

   
}

void MapGraphicsView::DrawMap() {

    // Load cities and connections
    auto citiesMap = Game::getInstance().GetMap()->GetCities();
    auto connections = Game::getInstance().GetMap()->GetConnections();

    // Reset maps
    citiesItemsMap = map<string, shared_ptr<CityItem>>();
    connectionItems = vector<std::unique_ptr<ConnectionItem>>();

    // Reset scene
    graphicsScene = std::make_unique<QGraphicsScene>();
    setScene(graphicsScene.get());

    // Create cities items
    for (auto city : citiesMap) {

        citiesItemsMap[city.first] = std::make_shared<CityItem>(QPoint(city.second->getX(),
                                                                        city.second->getY()), city.second->getWidth(), city.second->getHeight());
        citiesItemsMap[city.first]->SetName(city.first);
        citiesItemsMap[city.first]->SetCity(city.second);
        citiesItemsMap[city.first]->SetRegionColor(QColor(city.second->GetRegion()->GetName().c_str()));
    }

    // Create connections items
    QFont connectionFont = QFont("Tahoma", 7, QFont::Bold);
    QFont cityFont = QFont("Calibri", 9, QFont::Bold, true);
    for (auto connection : connections) {

        // Only add the connection item if both cities are inside enabled regions
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
    for (auto cityItem : citiesItemsMap) {
        auto cityNameTextItem = cityItem.second->GetNameTextItem(cityFont);
        scene()->addItem(cityNameTextItem);

        scene()->addItem(cityItem.second.get());
    }
}