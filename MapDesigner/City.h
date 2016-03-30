#pragma once

#include <QGraphicsEllipseItem>

class City : public QGraphicsEllipseItem
{
    std::string name;
    QColor regionColor;

public:
    City(QPoint center, int width, int height);

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    void SetRegionColor(QColor regionColor) { this->regionColor = regionColor; }
    QColor GetRegionColor() const { return regionColor; }

    QPoint GetNameLocation(QFont font) const;
    QPoint GetCenter() const;
    QGraphicsSimpleTextItem* GetNameTextItem(QFont font) const;
};

