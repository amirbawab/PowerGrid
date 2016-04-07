#include "MapGraphicsView.h"

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

string MapGraphicsView::GetCityByPoint(QPoint point)
{
    for (auto cityItem : citiesItemsMap)
        if (cityItem.second->contains(point))
            return cityItem.first;

    return "";
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
    // If not selecting cities
    if (!selectCity && !selectRegions)
        QGraphicsView::mousePressEvent(event);

    // Only process left clicks
    if (event->button() != Qt::LeftButton)
        return;

    // 'mapToScene' returns QPointF; convert to QPoint
    auto scenePoint = mapToScene(event->pos());
    auto point = QPoint(scenePoint.x(), scenePoint.y());

    if (selectCity)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
            return;

        // Refresh map
        selectedCity = citiesItemsMap[cityName]->GetCity();
        DrawMap();

        selectCity = false;
        emit CitySelected(selectedCity);
        return;
    }

    if (selectRegions)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
            return;

        // Add city's region to the list if it's not already
        auto cityRegion = citiesItemsMap[cityName]->GetCity()->GetRegion();
        auto regionIt = std::find(selectedRegions.begin(), selectedRegions.end(), cityRegion);
        if (regionIt == selectedRegions.end())
            selectedRegions.push_back(citiesItemsMap[cityName]->GetCity()->GetRegion());

        // Refresh map
        DrawMap();

        // If enough regions are selected, emit signal
        if (selectedRegions.size() == numberOfRegions)
        {
            DrawMap();
            selectRegions = false;
            emit RegionsSelected(selectedRegions);
        }
    }
}

void MapGraphicsView::OnSelectCity()
{
    selectedCity.reset();
    selectCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
}

void MapGraphicsView::OnSelectRegions(int count)
{
    selectRegions = true;
    numberOfRegions = count;
    selectedRegions = vector<shared_ptr<Region>>();
    viewport()->setCursor(Qt::ArrowCursor);
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

        // Only add the city item if city's region is enabled
        if (city.second->GetRegion()->IsEnabled())
        {
            citiesItemsMap[city.first] = std::make_shared<CityItem>(QPoint(city.second->getX(),
                                                                           city.second->getY()), city.second->getWidth(), city.second->getHeight());
            citiesItemsMap[city.first]->SetName(city.first);
            citiesItemsMap[city.first]->SetCity(city.second);
            citiesItemsMap[city.first]->SetRegionColor(QColor(city.second->GetRegion()->GetName().c_str()));
        }
    }

    // Create connections items
    QFont connectionFont = QFont("Tahoma", 7, QFont::Bold);
    QFont cityFont = QFont("Calibri", 9, QFont::Bold, true);
    for (auto connection : connections) {

        // Only add the connection item if both cities are inside enabled regions
        if (connection->GetFirst()->GetRegion()->IsEnabled() && connection->GetSecond()->GetRegion()->IsEnabled())
        {
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
    }

    // Ading cities
    for (auto cityItem : citiesItemsMap) {
        auto cityNameTextItem = cityItem.second->GetNameTextItem(cityFont);
        scene()->addItem(cityNameTextItem);

        // Highlight selected city
        if (selectedCity && cityItem.second->GetCity() == selectedCity)
            scene()->addRect(cityItem.second->rect(), QPen(Qt::white, 2));

        // Highlight selected regions
        if (selectedRegions.size() > 0)
        {
            auto regionIt = std::find(selectedRegions.begin(), selectedRegions.end(),
                                      cityItem.second->GetCity()->GetRegion());
            if (regionIt != selectedRegions.end())
                scene()->addRect(cityItem.second->rect(), QPen(Qt::white, 2));
        }

        scene()->addItem(cityItem.second.get());
    }
}

void MapGraphicsView::Reset()
{
    selectedRegions = vector<shared_ptr<Region>>();
    selectedCity.reset();
}
