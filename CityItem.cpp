#include "CityItem.h"
#include <QFont>
#include <QFontMetrics>

//#include <iostream>
//using std::cout;
//using std::endl;

CityItem::CityItem(QPoint topLeft, int width, int height) {
    setRect(topLeft.x(), topLeft.y(), width, height);
}

void CityItem::SetRegionColor(QColor regionColor) {
    this->regionColor = regionColor;
    setBrush(QBrush(regionColor));
}

QPoint CityItem::GetNameLocation(QFont font) const {
    int nameOffset = 2;

    QFontMetrics metrics(font);
    auto nameWidth = metrics.width(QString::fromStdString(name));
    auto nameHeight = metrics.height();

    return QPoint(GetCenter().x() - nameWidth / 2,
        GetCenter().y() - rect().height() / 2 - nameHeight - nameOffset);
}

QPoint CityItem::GetCenter() const {
    return QPoint(city->getX() + city->getWidth() / 2, city->getY() + city->getHeight() / 2);
}

QGraphicsSimpleTextItem* CityItem::GetNameTextItem(QFont font) const {
    auto cityNameTextItem = new QGraphicsSimpleTextItem(name.c_str());
    cityNameTextItem->setPos(GetNameLocation(font));
    cityNameTextItem->setFont(font);
    cityNameTextItem->setBrush(QBrush(Qt::white));

    return cityNameTextItem;
}

QPoint CityItem::GetHousePosition(int index) const {
    auto center = GetCenter();
    int offset = 10;

    switch (index) {
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
