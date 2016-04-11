#include "CityItem.h"
#include <QBrush>
#include <QFontMetrics>

#include <iostream>
using std::cout;
using std::endl;

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
//    return QPoint(city->getX() + city->getWidth() / 2, city->getY() + city->getHeight() / 2);
    return QPoint(rect().x() + rect().width() / 2, rect().y() + rect().height() / 2);
}

QGraphicsSimpleTextItem* CityItem::GetNameTextItem(QFont font) const {
    auto cityNameTextItem = new QGraphicsSimpleTextItem(name.c_str());
    cityNameTextItem->setPos(GetNameLocation(font));
    cityNameTextItem->setFont(font);
    cityNameTextItem->setBrush(QBrush(Qt::white));

    return cityNameTextItem;
}

QPoint CityItem::GetHousePosition(int index, QGraphicsPixmapItem* houseItem) const {
    auto center = GetCenter();
    int offset = 20;
    auto houseItemSize = QSize(houseItem->boundingRect().width(), houseItem->boundingRect().height());
    auto houseCenterPosition = QPoint(center.x() - houseItemSize.width() / 2,
                                      center.y() - houseItemSize.height() / 2);

    switch (index) {
        // First house
    case 0:
        return houseCenterPosition + QPoint(0, -offset);
        // Second house
    case 1:
        return houseCenterPosition + QPoint(-offset, +offset);
        // Default is third house!
    default:
        return houseCenterPosition + QPoint(+offset, +offset);
    }
}
