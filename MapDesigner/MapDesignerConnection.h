#pragma once

#include <QGraphicsLineItem>
#include "MapDesignerCity.h"
#include <memory>

class MapDesignerConnection : public QGraphicsLineItem
{
    const int LINE_WIDTH = 5;
    const QColor LINE_COLOR = Qt::blue;

    std::shared_ptr<MapDesignerCity> first;
    std::shared_ptr<MapDesignerCity> second;
    int cost = 0;

public:
    MapDesignerConnection() {}

    QPoint GetCostLocation(QFont font) const;
    int GetCostCircleDiameter(QFont font) const;

    void SetFirstCity(std::shared_ptr<MapDesignerCity> city) { this->first = city; }
    std::shared_ptr<MapDesignerCity> GetFirstCity() const { return first; }
    void SetSecondCity(std::shared_ptr<MapDesignerCity> city);
    std::shared_ptr<MapDesignerCity> GetSecondCity() const { return second; }

    void SetCost(int cost) { this->cost = cost; }
    int GetCost() const { return cost; }

    QSize GetCostPixelSize(QFont font) const;
    QGraphicsSimpleTextItem* GetCostTextItem(QFont font) const;
    QGraphicsEllipseItem* GetCostEllipseItem(QFont font) const;
};

