#include "MapDesignerGraphicsView.h"
#include <iostream>
#include <QResizeEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <time.h>
#include <QColorDialog>

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
    setScene(new QGraphicsScene());

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
    string cityName = "";

    for (auto cityMapItem : cities)
        if (cityMapItem.second->contains(point))
            cityName = cityMapItem.first;

    return cityName;
}

void MapDesignerGraphicsView::resizeEvent(QResizeEvent*)
{
    setSceneRect(rect());
}

void MapDesignerGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (addCity)
    {
        auto point = event->pos();
        bool ok;
        releaseKeyboard();
        auto cityName = QInputDialog::getText(this, "City Name",
                                              "Please specify the city name:",
                                              QLineEdit::Normal, "", &ok);
        grabKeyboard();

        if (!ok || cityName.isEmpty())
        {
            emit ClearMessage();
            return;
        }

        auto city = make_shared<City>(point, CITY_WIDTH, CITY_HEIGHT);
        city->SetName(cityName.toStdString());
        city->setBrush(QBrush(regionColor));

        if (cities.find(cityName.toStdString()) != cities.end())
        {
            QMessageBox::warning(this, "Duplicate City Name",
                                 "City with name '" + cityName + "' already exists!");
            OnCancelOperation();
            return;
        }
        cities[cityName.toStdString()] = city;
        srand(time(nullptr));
        int random = rand() % 3;
        for (int i = 0; i <= random; i++)
        {
            auto house = make_shared<PlayerHouse>();
            city->GetHouses().push_back(house);
        }

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
        auto point = event->pos();

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
        auto point = event->pos();

        // If no city contains this point
        auto cityName = GetCityByPoint(point);
        if (cityName == "")
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

MapDesignerGraphicsView::MapDesignerGraphicsView()
{
    cityFont = QFont("Calibri", 12, QFont::Bold, true);
    connectionFont = QFont("Tahoma", 12, QFont::Bold);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        this->regionColor = *color;
        return true;
    }

    // If region colors is not full, we allow
    if (regionColors.size() < 6)
    {
        this->regionColor = regionColor;
        return true;
    }

    // Otherwise, we only allow if the color is already in the list
    if (color != regionColors.end())
        return true;

    return false;
}

void MapDesignerGraphicsView::OnAddCity()
{
    if (regionColor == Qt::white)
        return;

    addCity = true;
    emit DisplayMessage("Please select the center of the city ...");
}

void MapDesignerGraphicsView::OnAddConnection()
{
    if (regionColor == Qt::white)
        return;

    addConnectionFirstCity = true;
    emit DisplayMessage("Please select the first city ...");
}

void MapDesignerGraphicsView::OnCancelOperation()
{
    addCity = false;
    addConnectionFirstCity = addConnectionSecondCity = false;
    emit ClearMessage();
}
