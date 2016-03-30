#pragma once

#include <QGraphicsLineItem>
#include "City.h"
#include <memory>

class Connection : public QGraphicsLineItem
{
    std::shared_ptr<City> first;
    std::shared_ptr<City> second;
    int cost = 0;

public:
    Connection() {}

    QPoint GetCostLocation(QFont font) const;
    int GetCostCircleDiameter(QFont font) const;

    void SetFirstCity(std::shared_ptr<City> city) { this->first = city; }
    void SetSecondCity(std::shared_ptr<City> city);

    void SetCost(int cost) { this->cost = cost; }
    int GetCost() const { return cost; }

    QSize GetCostPixelSize(QFont font) const;
};

