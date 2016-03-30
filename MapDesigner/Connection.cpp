#include "Connection.h"
#include <QFont>
#include <QFontMetrics>

QPoint Connection::GetCostLocation(QFont font) const
{
    int deltaY = first->GetCenter().y() - second->GetCenter().y();
    int deltaX = first->GetCenter().x() - second->GetCenter().x();

    auto costSize = GetCostPixelSize(font);
    int centerX = abs(deltaX) / 2 - costSize.width() / 2;
    int centerY = abs(deltaY) / 2 - costSize.height() / 2;

    return QPoint(std::min(first->GetCenter().x(), second->GetCenter().x()) + centerX,
                  std::min(first->GetCenter().y(), second->GetCenter().y()) + centerY);
}

int Connection::GetCostCircleDiameter(QFont font) const
{
    auto diameterOffset = 10;

    auto costSize = GetCostPixelSize(font);
    return std::max(costSize.width(), costSize.height()) + diameterOffset;
}

void Connection::SetSecondCity(std::shared_ptr<City> city)
{
    if (!first)
        return;

    this->second = city;
    setLine(first->GetCenter().x(), first->GetCenter().y(),
            second->GetCenter().x(), second->GetCenter().y());
}

QSize Connection::GetCostPixelSize(QFont font) const
{
    auto costString = std::to_string(cost);
    QFontMetrics metrics(font);

    auto costPixelWidth = metrics.width(costString.c_str());
    auto costPixelHeight = metrics.height();
    return QSize(costPixelWidth, costPixelHeight);
}
