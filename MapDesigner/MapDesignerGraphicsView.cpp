#include "MapDesignerGraphicsView.h"
#include <iostream>
#include <QResizeEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

void MapDesignerGraphicsView::UpdateScene()
{
    graphicsScene.release();
    graphicsScene = make_unique<QGraphicsScene>();
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

        auto city = make_shared<City>(point, CITY_WIDTH, CITY_HEIGHT);
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

    if (addConnectionFirstCity)
    {
        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
            return;

        connection = make_unique<Connection>();
        connection->SetFirstCity(cities[cityName]);

        addConnectionFirstCity = false;
        addConnectionSecondCity = true;

        emit DisplayMessage("Now, please select the second city ...");
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
    graphicsScene = make_unique<QGraphicsScene>();
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

    emit ClearMessage();
    addCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
    emit DisplayMessage("Please select the center of the city ...");
}

void MapDesignerGraphicsView::OnAddConnection()
{
    if (cities.size() < 2)
    {
        emit DisplayMessage("There must be at least two cities on the map");
        return;
    }

    emit ClearMessage();
    addConnectionFirstCity = true;
    viewport()->setCursor(Qt::ArrowCursor);
    emit DisplayMessage("Please select the first city ...");
}

void MapDesignerGraphicsView::OnCancelOperation()
{
    addCity = false;
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
    PopulateCities(map);
    PopulateConnections(map);

    if (document.save_file(fileName.toStdString().c_str()))
        QMessageBox::information(this, "Success", "XML file saved successfully!");
    else
        QMessageBox::critical(this, "Error", "Could not save the XML file!");
}

void MapDesignerGraphicsView::PopulateCities(pugi::xml_node& map)
{
    auto citiesNode = map.append_child("cities");
    for (auto cityMapItem : cities)
    {
        auto city = cityMapItem.second;

        auto cityNode = citiesNode.append_child("city");
        auto cityNameAttribute = cityNode.append_attribute("name");
        auto cityRegionAttribute = cityNode.append_attribute("region");
        auto cityXAttribute = cityNode.append_attribute("x");
        auto cityYAttribute = cityNode.append_attribute("y");
        auto cityWidthAttribute = cityNode.append_attribute("width");
        auto cityHeightAttribute = cityNode.append_attribute("height");

        cityNameAttribute.set_value(city->GetName().c_str());
        cityRegionAttribute.set_value(city->GetRegionColor().name().toStdString().c_str());
        cityXAttribute.set_value(city->rect().x());
        cityYAttribute.set_value(city->rect().y());
        cityWidthAttribute.set_value(city->rect().width());
        cityHeightAttribute.set_value(city->rect().height());
    }
}

void MapDesignerGraphicsView::PopulateConnections(pugi::xml_node& map)
{
    auto connectionsNode = map.append_child("connections");
    for (auto& connection : connections)
    {
        auto connectionNode = connectionsNode.append_child("connection");
        auto connectionFirstAttribute = connectionNode.append_attribute("first");
        auto connectionSecondAttribute = connectionNode.append_attribute("second");
        auto connectionCostAttribute = connectionNode.append_attribute("cost");

        connectionFirstAttribute.set_value(connection->GetFirstCity()->GetName().c_str());
        connectionSecondAttribute.set_value(connection->GetSecondCity()->GetName().c_str());
        connectionCostAttribute.set_value(connection->GetCost());
    }
}
