#pragma once

#include <QGraphicsEllipseItem>
#include "MapDesignerPlayer.h"
#include <memory>

class MapDesignerCity : public QGraphicsEllipseItem
{
    std::string name;
    QColor regionColor;
    std::vector<std::shared_ptr<MapDesignerPlayer>> players;
    std::vector<std::shared_ptr<MapDesignerPlayerHouse>> houses;

public:
    MapDesignerCity(QPoint center, int width, int height);

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    std::vector<std::shared_ptr<MapDesignerPlayer>>& GetPlayers() { return players; }
    std::vector<std::shared_ptr<MapDesignerPlayerHouse>>& GetHouses() { return houses; }

    void SetRegionColor(QColor regionColor);
    QColor GetRegionColor() const { return regionColor; }

    QPoint GetNameLocation(QFont font) const;
    QPoint GetCenter() const;
    QGraphicsSimpleTextItem* GetNameTextItem(QFont font) const;

    QPoint GetHousePosition(int index) const;
};

