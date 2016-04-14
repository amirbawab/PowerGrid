#include "MapDesignerGraphicsView.h"
#include <iostream>
#include <QResizeEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include "Map.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::map;
using std::stoi;
using std::set;

void MapDesignerGraphicsView::UpdateScene()
{
    graphicsScene.release();
    graphicsScene = make_unique<QGraphicsScene>(this);
    setScene(graphicsScene.get());

    for (auto& connection : connections)
    {
        auto costTextItem = connection->GetCostTextItem(connectionFont);
        auto costEllipseItem = connection->GetCostEllipseItem(connectionFont);

        scene()->addItem(connection.get());
        scene()->addItem(costEllipseItem);
        scene()->addItem(costTextItem);
    }

    for (auto cityMapItem : cities)
    {
        auto cityNameTextItem = cityMapItem.second->GetNameTextItem(cityFont);
        scene()->addItem(cityNameTextItem);
        scene()->addItem(cityMapItem.second.get());

        for (int i = 0; i < cityMapItem.second->GetHouses().size(); i++)
        {
            auto houseLocation = cityMapItem.second->GetHousePosition(i);
            auto houseItem = cityMapItem.second->GetHouses()[i];
            houseItem->setRect(houseLocation.x() - houseItem->rect().width() / 2,
                               houseLocation.y() - houseItem->rect().height() / 2,
                               houseItem->rect().width(), houseItem->rect().height());
            scene()->addItem(houseItem.get());
        }
    }
}

string MapDesignerGraphicsView::GetCityByPoint(QPoint point)
{
    for (auto cityMapItem : cities)
        if (cityMapItem.second->contains(point))
            return cityMapItem.first;

    return "";
}

void MapDesignerGraphicsView::ResetScale()
{
    while (scaleSteps != 0)
    {
        if (scaleSteps > 0)
        {
            scale(1 / scaleFactor, 1 / scaleFactor);
            scaleSteps--;
        }
        else
        {
            scale(scaleFactor, scaleFactor);
            scaleSteps++;
        }
    }
}

void MapDesignerGraphicsView::resizeEvent(QResizeEvent*)
{
    setSceneRect(rect());
}

void MapDesignerGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (!addCity && !addConnectionFirstCity && !addConnectionSecondCity)
        QGraphicsView::mousePressEvent(event);

    if (event->button() != Qt::LeftButton)
        return;

    // 'mapToScene' returns QPointF; convert to QPoint
    auto scenePoint = mapToScene(event->pos());
    auto point = QPoint(scenePoint.x(), scenePoint.y());

    if (addCity)
    {
        // If thre's already a city at this point
        auto existingCityName = GetCityByPoint(point);
        if (existingCityName != "")
        {
            QMessageBox::critical(this, "Duplicate City",
                                 "There's already a city here! Please select another point");
            return;
        }

        // Check to see if the rectangle for the new city intersects the rectangle
        //      for an existing city
        QRect newCityRectangle(point.x() - CITY_WIDTH / 2,
                               point.y() - CITY_HEIGHT / 2,
                               CITY_WIDTH, CITY_HEIGHT);
        for (auto city : cities)
        {
            auto cityRectangle = QRect(city.second->rect().x(), city.second->rect().y(),
                                        city.second->rect().width(), city.second->rect().height());
            if (newCityRectangle.intersects(cityRectangle))
            {
                QMessageBox::critical(this, "Error",
                                      QString::fromStdString("Specified location is to close to city '" +
                                      city.second->GetName() + "'"));
                return;
            }
        }

        bool ok;
        releaseKeyboard();
        auto cityName = QInputDialog::getText(this, "City Name",
                                              "Please specify the city name:",
                                              QLineEdit::Normal, "", &ok);
        grabKeyboard();

        if (!ok || cityName.isEmpty())
        {
            emit DisplayMessage("You must specify a name for the city!");
            return;
        }

        auto city = make_shared<MapDesignerCity>(point, CITY_WIDTH, CITY_HEIGHT);
        city->SetRegionColor(regionColor);
        city->SetName(cityName.toStdString());

        if (cities.find(cityName.toStdString()) != cities.end())
        {
            QMessageBox::warning(this, "Duplicate City Name",
                                 "City with name '" + cityName + "' already exists!");
            OnCancelOperation();
            return;
        }
        cities[cityName.toStdString()] = city;

        // Only add the region color if it's not already in the list
        if (std::find(regionColors.begin(), regionColors.end(), regionColor) == regionColors.end())
        {
            QColorDialog::setCustomColor(regionColors.size(), regionColor);
            regionColors.push_back(regionColor);
        }

        addCity = false;
        emit ClearMessage();

        UpdateScene();
    }

    if (deleteCity)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
            return;

        auto reply = QMessageBox::question(this, "Confirm Deletion",
                                           QString("Are you sure you want to ")
                                           .append("delete city '").append(cityName.c_str())
                                           .append("'"), QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::No);
        if (reply == QMessageBox::No)
            return;

        cities.erase(cityName);
        for (int i = 0; i < connections.size(); i++)
        {
            if (connections[i]->GetFirstCity()->GetName() == cityName ||
                connections[i]->GetSecondCity()->GetName() == cityName)

                connections.erase(connections.begin() + i);
        }

        deleteCity = false;
        emit ClearMessage();

        UpdateScene();
    }

    if (addConnectionFirstCity)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
            return;

        connection = make_unique<MapDesignerConnection>();
        connection->SetFirstCity(cities[cityName]);

        addConnectionFirstCity = false;
        addConnectionSecondCity = true;

        emit DisplayMessage("Now, please select the second city ... (Press <b>ESC</b> to Cancel)");
    }
    else if (addConnectionSecondCity)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "" || cityName == connection->GetFirstCity()->GetName())
            return;

        bool ok;
        releaseKeyboard();
        auto cost = QInputDialog::getInt(this, "Connection Cost",
                                         "Please enter the cost of this connection:", 5, 0, 50, 1, &ok);
        grabKeyboard();
        if (!ok)
        {
            emit ClearMessage();
            return;
        }

        connection->SetSecondCity(cities[cityName]);
        connection->SetCost(cost);

        for (auto& conn : connections)
        {
            auto firstCityName  = conn->GetFirstCity()->GetName();
            auto secondCityName = conn->GetSecondCity()->GetName();

            auto currentFirstCityName  = connection->GetFirstCity()->GetName();
            auto currentSecondCityName = connection->GetSecondCity()->GetName();

            if ((firstCityName == currentFirstCityName && secondCityName == currentSecondCityName) ||
                (firstCityName == currentSecondCityName && secondCityName == currentFirstCityName))
            {
                QMessageBox::warning(this, "Duplicate Connection Entry", QString::fromStdString(
                                     "There's already a connection between '" + currentFirstCityName +
                                     "' and '" + currentSecondCityName + "'"));
                OnCancelOperation();
                return;
            }
        }
        connections.push_back(std::move(connection));

        addConnectionSecondCity = false;
        emit ClearMessage();

        UpdateScene();
    }
}

void MapDesignerGraphicsView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        OnCancelOperation();
}

void MapDesignerGraphicsView::wheelEvent(QWheelEvent* event)
{
    // Don't zoom if nothing is in the scene
    if (scene()->items().size() == 0)
        return;

    setTransformationAnchor(AnchorUnderMouse);
    setDragMode(ScrollHandDrag);

    if (event->delta() > 0)
    {
        scaleSteps++;
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        scaleSteps--;
        scale(1 / scaleFactor, 1 / scaleFactor);
    }
}

MapDesignerGraphicsView::MapDesignerGraphicsView()
{
    graphicsScene = make_unique<QGraphicsScene>(this);
    setScene(graphicsScene.get());

    cityFont = QFont("Calibri", 12, QFont::Bold, true);
    connectionFont = QFont("Tahoma", 12, QFont::Bold);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    grabKeyboard();
}

bool MapDesignerGraphicsView::SetRegionColor(QColor regionColor)
{
    // If the default color, allow it
    if (regionColor == DEFAULT_REGION_COLOR)
    {
        this->regionColor = regionColor;
        return true;
    }

    auto color = std::find(regionColors.begin(), regionColors.end(), regionColor);

    // If the color is already in the selected colors
    if (color != regionColors.end())
    {
        emit ClearMessage();
        this->regionColor = *color;
        return true;
    }

    // If region colors is not full, we allow
    if (regionColors.size() < 6)
    {
        emit ClearMessage();
        this->regionColor = regionColor;
        return true;
    }

    // Otherwise, we only allow if the color is already in the list
    if (color != regionColors.end())
    {
        emit ClearMessage();
        return true;
    }

    return false;
}

void MapDesignerGraphicsView::OnAddCity()
{
    if (regionColor == Qt::white)
    {
        emit DisplayMessage("Please select a non-default region color");
        return;
    }

    OnCancelOperation();
    emit ClearMessage();
    addCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
    emit DisplayMessage("Please select the center of the city ... (Press <b>ESC</b> to Cancel)");
}

void MapDesignerGraphicsView::OnDeleteCity()
{
    if (cities.size() == 0)
    {
        emit DisplayMessage("There are no cities on the map!");
        return;
    }

    OnCancelOperation();
    emit ClearMessage();
    deleteCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
    emit DisplayMessage("Please select the city to delete ... (Press <b>ESC</b> to Cancel)");
}

void MapDesignerGraphicsView::OnAddConnection()
{
    if (cities.size() < 2)
    {
        emit DisplayMessage("There must be at least two cities on the map");
        return;
    }

    OnCancelOperation();
    emit ClearMessage();
    addConnectionFirstCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
    emit DisplayMessage("Please select the first city ... (Press <b>ESC</b> to Cancel)");
}

void MapDesignerGraphicsView::OnCancelOperation()
{
    addCity = false;
    deleteCity = false;
    addConnectionFirstCity = addConnectionSecondCity = false;
    emit ClearMessage();
}

void MapDesignerGraphicsView::OnExportXml()
{
    releaseKeyboard();
    auto mapName = QInputDialog::getText(this, "Map Name", "Please enter the map name");
    if (mapName.isEmpty())
    {
        grabKeyboard();
        return;
    }

    releaseKeyboard();
    auto fileName = QFileDialog::getSaveFileName(this, "XML File Location",
                                                 "", "XML Files (*.xml)");

    if (fileName.isEmpty())
    {
        grabKeyboard();
        return;
    }

    pugi::xml_document document;
    auto map = document.append_child("map");
    map.append_attribute("name").set_value(mapName.toStdString().c_str());
    map.append_attribute("description").set_value(MAP_DESCRIPTION.c_str());
    PopulateCities(map);
    PopulateConnections(map);

    if (document.save_file(fileName.toStdString().c_str()))
        QMessageBox::information(this, "Success", "XML file saved successfully!");
    else
        QMessageBox::critical(this, "Error", "Could not save the XML file!");
}

void MapDesignerGraphicsView::LoadXml(QString fileName)
{
    pugi::xml_document mapXml;
    QFile mapXmlFile(fileName);

    if (!mapXmlFile.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, "File Error", "Could not open file");
        return;
    }

    QString mapXmlContent = mapXmlFile.readAll();

    auto result = mapXml.load_string(mapXmlContent.toStdString().c_str());
    if (result.status != pugi::status_ok) {
        QMessageBox::critical(this, "XML Error",
                              QString("Could not read XML content for map, Reason: ")
                              .append(result.description()));
        return;
    }

    if (!Map::IsMapFileValid(fileName.toStdString()))
    {
        QMessageBox::critical(this, "Content Error", QString("This file doesn't appear")
                              .append(" to be created using PG MapDesigner!"));
        return;
    }

    if (!LoadCities(mapXml))
    {
        QMessageBox::critical(this, "City Input Error",
                              "Could not read city data from file");
        return;
    }

    if (!LoadConnections(mapXml))
    {
        QMessageBox::critical(this, "Connection Input Error",
                              "Could not read connection data from file");
        return;
    }

    UpdateScene();
}

void MapDesignerGraphicsView::PopulateCities(pugi::xml_node& map)
{
    auto citiesNode = map.append_child("cities");
    for (auto cityMapItem : cities)
    {
        auto city = cityMapItem.second;

        auto cityNode = citiesNode.append_child("city");
        cityNode.append_attribute("name").set_value(city->GetName().c_str());
        cityNode.append_attribute("region").set_value(city->GetRegionColor().name().toStdString().c_str());
        cityNode.append_attribute("x").set_value(city->rect().x());
        cityNode.append_attribute("y").set_value(city->rect().y());
        cityNode.append_attribute("width").set_value(city->rect().width());
        cityNode.append_attribute("height").set_value(city->rect().height());
    }
}

void MapDesignerGraphicsView::PopulateConnections(pugi::xml_node& map)
{
    auto connectionsNode = map.append_child("connections");
    for (auto& connection : connections)
    {
        auto connectionNode = connectionsNode.append_child("connection");
        connectionNode.append_attribute("first").set_value(connection->GetFirstCity()->GetName().c_str());
        connectionNode.append_attribute("second").set_value(connection->GetSecondCity()->GetName().c_str());
        connectionNode.append_attribute("cost").set_value(connection->GetCost());
    }
}

bool MapDesignerGraphicsView::LoadCities(pugi::xml_document& xml)
{
    cities = map<string, shared_ptr<MapDesignerCity>>();
    loadedRegionColors = set<QColor>();

    for (auto cityNode : xml.select_nodes("/map/cities/city"))
    {
        string cityName = cityNode.node().attribute("name").value();
        string regionColorName = cityNode.node().attribute("region").value();
        auto cityX = stoi(cityNode.node().attribute("x").value());
        auto cityY = stoi(cityNode.node().attribute("y").value());
        auto cityWidth = stoi(cityNode.node().attribute("width").value());
        auto cityHeight = stoi(cityNode.node().attribute("height").value());

        auto cityCenterValue = QPoint(cityX + cityWidth / 2, cityY + cityHeight / 2);
        QColor regionColor;
        regionColor.setNamedColor(regionColorName.c_str());

        auto city = make_shared<MapDesignerCity>(cityCenterValue, cityWidth, cityHeight);
        city->SetName(cityName);
        city->SetRegionColor(regionColor);

//        loadedRegionColors.insert(regionColor);

        // Add city to map
        cities[cityName] = city;
    }

    return true;
}

bool MapDesignerGraphicsView::LoadConnections(pugi::xml_document& xml)
{
    connections = vector<unique_ptr<MapDesignerConnection>>();

    string firstCity, secondCity;
    int cost;
    for (auto connectionNode : xml.select_nodes("//connections/connection"))
    {
        firstCity = connectionNode.node().attribute("first").value();
        secondCity = connectionNode.node().attribute("second").value();
        cost = stoi(connectionNode.node().attribute("cost").value());

        auto first = cities[firstCity];
        auto second = cities[secondCity];

        auto connection = make_unique<MapDesignerConnection>();
        connection->SetFirstCity(cities[firstCity]);
        connection->SetSecondCity(cities[secondCity]);
        connection->SetCost(cost);

        connections.push_back(std::move(connection));
    }

    return true;
}
