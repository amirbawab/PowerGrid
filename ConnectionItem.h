#pragma once

#include <QGraphicsLineItem>
#include "Connection.h"
#include "CityItem.h"
#include <memory>

class ConnectionItem : public QGraphicsLineItem {
    const int LINE_WIDTH = 5;
    const QColor LINE_COLOR = Qt::blue;
    std::shared_ptr<Connection> connection;
    std::shared_ptr<CityItem> first;
    std::shared_ptr<CityItem> second;

public:
    ConnectionItem() {}

    QPoint GetCostLocation(QFont font) const;
    int GetCostCircleDiameter(QFont font) const;

    void SetConnection(std::shared_ptr<Connection> connection) { this->connection = connection; }
    std::shared_ptr<Connection> GetConnection() { return connection; }

    void SetFirstCity(std::shared_ptr<CityItem> city) { this->first = city; }
    std::shared_ptr<CityItem> GetFirstCity() const { return first; }
    void SetSecondCity(std::shared_ptr<CityItem> city);
    std::shared_ptr<CityItem> GetSecondCity() const { return second; }

    QSize GetCostPixelSize(QFont font) const;
    QGraphicsSimpleTextItem* GetCostTextItem(QFont font) const;
    QGraphicsEllipseItem* GetCostEllipseItem(QFont font) const;
};

