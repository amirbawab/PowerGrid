#pragma once

#include <QGraphicsEllipseItem>
#include <Player.h>
#include <memory>

class CityItem : public QGraphicsEllipseItem {
    std::string name;
    QColor regionColor;
   
public:
    CityItem(QPoint topLeft, int width, int height);

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    void SetRegionColor(QColor regionColor);
    QColor GetRegionColor() const { return regionColor; }

    QPoint GetNameLocation(QFont font) const;
    QPoint GetCenter() const;
    QGraphicsSimpleTextItem* GetNameTextItem(QFont font) const;

    QPoint GetHousePosition(int index) const;
};

