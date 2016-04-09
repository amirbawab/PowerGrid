#include "ConnectionItem.h"
#include <QPen>
#include <QFont>
#include <QFontMetrics>

QPoint ConnectionItem::GetCostLocation(QFont font) const {
    int deltaY = first->GetCenter().y() - second->GetCenter().y();
    int deltaX = first->GetCenter().x() - second->GetCenter().x();

    auto costSize = GetCostPixelSize(font);
    int centerX = abs(deltaX) / 2 - costSize.width() / 2;
    int centerY = abs(deltaY) / 2 - costSize.height() / 2;

    return QPoint(std::min(first->GetCenter().x(), second->GetCenter().x()) + centerX,
        std::min(first->GetCenter().y(), second->GetCenter().y()) + centerY);
}

int ConnectionItem::GetCostCircleDiameter(QFont font) const {
    auto diameterOffset = 8;

    auto costSize = GetCostPixelSize(font);
    return std::max(costSize.width(), costSize.height()) + diameterOffset;
}

void ConnectionItem::SetSecondCity(std::shared_ptr<CityItem> city) {
    if (!first)
        return;

    this->second = city;
    setLine(first->GetCenter().x(), first->GetCenter().y(),
        second->GetCenter().x(), second->GetCenter().y());
    setPen(QPen(LINE_COLOR, LINE_WIDTH));
}

QSize ConnectionItem::GetCostPixelSize(QFont font) const {
    auto costString = std::to_string(connection->GetCost());
    QFontMetrics metrics(font);

    auto costPixelWidth = metrics.width(costString.c_str());
    auto costPixelHeight = metrics.height();
    return QSize(costPixelWidth, costPixelHeight);
}

QGraphicsSimpleTextItem* ConnectionItem::GetCostTextItem(QFont font) const {
    auto costString = std::to_string(connection->GetCost());
    auto costLocation = GetCostLocation(font);

    auto costTextItem = new QGraphicsSimpleTextItem(QString::fromStdString(costString));
    costTextItem->setPos(costLocation);
    costTextItem->setFont(font);

    return costTextItem;
}

QGraphicsEllipseItem* ConnectionItem::GetCostEllipseItem(QFont font) const {
    auto costLocation = GetCostLocation(font);
    auto costSize = GetCostPixelSize(font);
    auto costDiameter = GetCostCircleDiameter(font);

    auto costCircle = new QGraphicsEllipseItem(costLocation.x() + costSize.width() / 2 - costDiameter / 2,
        costLocation.y() + costSize.height() / 2 - costDiameter / 2,
        costDiameter, costDiameter);
    costCircle->setBrush(QBrush(Qt::white));
    costCircle->setPen(QPen(LINE_COLOR, LINE_WIDTH / 2));

    return costCircle;
}
