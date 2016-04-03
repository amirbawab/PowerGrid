#include "City.h"
#include <QFont>
#include <QFontMetrics>

//#include <iostream>
//using std::cout;
//using std::endl;

City::City(QPoint center, int width, int height)
{
    setRect(center.x() - width / 2, center.y() - height / 2, width, height);
}

void City::SetRegionColor(QColor regionColor)
{
    this->regionColor = regionColor;
    setBrush(QBrush(regionColor));
}

QPoint City::GetNameLocation(QFont font) const
{
    int nameOffset = 5;

    QFontMetrics metrics(font);
    auto nameWidth = metrics.width(QString::fromStdString(name));
    auto nameHeight = metrics.height();

    return QPoint(GetCenter().x() - nameWidth / 2,
                  GetCenter().y() - rect().height() / 2 - nameHeight - nameOffset);
}

QPoint City::GetCenter() const
{
    return QPoint(rect().x() + rect().width() / 2, rect().y() + rect().height() / 2);
}

QGraphicsSimpleTextItem* City::GetNameTextItem(QFont font) const
{
    auto cityNameTextItem = new QGraphicsSimpleTextItem(name.c_str());
    cityNameTextItem->setPos(GetNameLocation(font));
    cityNameTextItem->setFont(font);
    
    return cityNameTextItem;
}

QPoint City::GetHousePosition(int index) const
{
    auto center = GetCenter();
    int offset = 10;

    switch (index)
    {
        // First house
        case 0:
            return QPoint(center.x(), center.y() - offset);
        // Second house
        case 1:
            return QPoint(center.x() - offset, center.y() + offset);
        // Default is third house!
        default:
            return QPoint(center.x() + offset, center.y() + offset);
    }
}
