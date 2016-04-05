#pragma once

#include <QGraphicsEllipseItem>
#include <Player.h>
#include <memory>

class City : public QGraphicsEllipseItem
{
    std::string name;
    QColor regionColor;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<PlayerHouse>> houses;

public:
    City(QPoint center, int width, int height);

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    std::vector<std::shared_ptr<Player>>& GetPlayers() { return players; }
    std::vector<std::shared_ptr<PlayerHouse>>& GetHouses() { return houses; }

    void SetRegionColor(QColor regionColor);
    QColor GetRegionColor() const { return regionColor; }

    QPoint GetNameLocation(QFont font) const;
    QPoint GetCenter() const;
    QGraphicsSimpleTextItem* GetNameTextItem(QFont font) const;

    QPoint GetHousePosition(int index) const;
};

