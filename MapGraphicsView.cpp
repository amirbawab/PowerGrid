#include "MapGraphicsView.h"
#include "Game.h"

MapGraphicsView::MapGraphicsView() {
    
    // Set id
    setObjectName("mapGraphicsView");

    // Init components
    graphicsScene = std::make_unique<QGraphicsScene>(this);

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
    for (auto cityItem : cityItemsMap)
        if (cityItem.second->contains(point))
            return cityItem.second->GetCity();

    return nullptr;
}

void MapGraphicsView::MarkCitiesToRemove(shared_ptr<City> city)
{
    for (auto cityItem : cityItemsMap) {
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

    // Only process left clicks, and only proceed if we need to select a city or a region
    if (event->button() != Qt::LeftButton ||
        !Game::getInstance().selectCity && !Game::getInstance().selectRegion)
        return;

    // Set the cursor
    viewport()->setCursor(Qt::ArrowCursor);

    // 'mapToScene' returns QPointF; convert to QPoint
    auto scenePoint = mapToScene(event->pos());
    auto point = QPoint(scenePoint.x(), scenePoint.y());

    // If no city contains this point
    auto city = GetCityByPoint(point);
    if (!city)
        return;

    // Store selected city/region
    if (Game::getInstance().selectCity)
    {
        selectedCity = city;
        selectedRegion.reset();
        DrawMap();
    }
    // Otherwise, selectRegion must be true
    else
    {
        selectedCity.reset();
        selectedRegion = city->GetRegion();
        DrawMap();
    }

    emit CitySelected(city);

    // If starting game, select cities to remove
    if (Game::getInstance().GetPhase() == 0) {
        MarkCitiesToRemove(city);
    }
    else {
    
    }
}

void MapGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    // Let the parent handle the event first
    QGraphicsView::mouseMoveEvent(event);

    if (Game::getInstance().selectCity || Game::getInstance().selectRegion)
        viewport()->setCursor(Qt::ArrowCursor);
    else
        viewport()->setCursor(Qt::OpenHandCursor);
}

void MapGraphicsView::Refresh() {
    
    

   
}

void MapGraphicsView::DrawMap() {

    // Load cities and connections
    auto citiesMap = Game::getInstance().GetMap()->GetCities();
    auto connections = Game::getInstance().GetMap()->GetConnections();

    // Reset maps
    cityItemsMap = map<string, shared_ptr<CityItem>>();
    connectionItems = vector<std::unique_ptr<ConnectionItem>>();
    houseItems = vector<std::unique_ptr<HouseItem>>();

    // Reset scene
    graphicsScene = std::make_unique<QGraphicsScene>(this);
    setScene(graphicsScene.get());

    // Create cities items
    for (auto city : citiesMap) {

        cityItemsMap[city.first] = std::make_shared<CityItem>(QPoint(city.second->getX(),
                                                                        city.second->getY()), city.second->getWidth(), city.second->getHeight());
        cityItemsMap[city.first]->SetName(city.first);
        cityItemsMap[city.first]->SetCity(city.second);
        cityItemsMap[city.first]->SetRegionColor(QColor(city.second->GetRegion()->GetName().c_str()));
    }

    // Create connections items
    QFont connectionFont = QFont("Tahoma", 7, QFont::Bold);
    QFont cityFont = QFont("Calibri", 9, QFont::Bold, true);
    for (auto connection : connections) {

        // Only add the connection item if both cities are inside enabled regions
        connectionItems.push_back(std::make_unique<ConnectionItem>());

        // Set cities
        auto connectionItem = connectionItems[connectionItems.size() - 1].get();
        connectionItem->SetFirstCity(cityItemsMap[connection->GetFirst()->GetName()]);
        connectionItem->SetSecondCity(cityItemsMap[connection->GetSecond()->GetName()]);

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
    for (auto cityItem : cityItemsMap) {
        auto cityNameTextItem = cityItem.second->GetNameTextItem(cityFont);
        scene()->addItem(cityNameTextItem);
        scene()->addItem(cityItem.second.get());

        // Highlight
        if (selectedCity && cityItem.second->GetCity() == selectedCity)
        {
            scene()->addRect(cityItem.second->rect(), QPen(Qt::white, 2));
            centerOn(cityItem.second.get());
        }
        if (selectedRegion && cityItem.second->GetCity()->GetRegion() == selectedRegion)
            scene()->addRect(cityItem.second->rect(), QPen(Qt::white, 2));

        // Add house(s)
        auto& houses = cityItem.second->GetCity()->GetHouses();
        int numberOfHouses = houses.size();
        for (auto i = 0; i < numberOfHouses; i++)
        {
            // Create house item
            houseItems.push_back(std::make_unique<HouseItem>());
            auto& houseItem = houseItems[houseItems.size() - 1];

            // Create image
            QImage houseImage(houses[i]->GetColor()->getImage().c_str());
            
            // Set pixmap properties
            houseItem->setPixmap(QPixmap::fromImage(houseImage));
            houseItem->setOffset(cityItem.second->GetHousePosition(i, houseItem.get()));

            // Add to the scene
            scene()->addItem(houseItem.get());
        }
    }
}

void MapGraphicsView::ResetSelected()
{
    selectedCity.reset();
    selectedRegion.reset();
}
