#pragma once

#include <QGraphicsLineItem>
#include "Connection.h"
#include "CityItem.h"
#include <memory>

class ConnectionItem : public QGraphicsLineItem {
    const int LINE_WIDTH = 5;
    const QColor LINE_COLOR = Qt::blue;
    shared_ptr<Connection> connection;
    shared_ptr<CityItem> first;
    shared_ptr<CityItem> second;

public:
    ConnectionItem() {}

    QPoint GetCostLocation(QFont font) const;
    int GetCostCircleDiameter(QFont font) const;

    void SetConnection(shared_ptr<Connection> connection) { this->connection = connection; }
    shared_ptr<Connection> GetConnection() const { return connection; }

    void SetFirstCity(shared_ptr<CityItem> city) { this->first = city; }
    shared_ptr<CityItem> GetFirstCity() const { return first; }
    void SetSecondCity(shared_ptr<CityItem> city);
    shared_ptr<CityItem> GetSecondCity() const { return second; }

    QSize GetCostPixelSize(QFont font) const;
    QGraphicsSimpleTextItem* GetCostTextItem(QFont font) const;
    QGraphicsEllipseItem* GetCostEllipseItem(QFont font) const;
};

