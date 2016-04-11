#pragma once

#include <QGraphicsItem>
#include "House.h"

class HouseItem : public QGraphicsPixmapItem
{
    std::shared_ptr<House> house;

public:
    explicit HouseItem(QGraphicsItem* parent = nullptr) :
        QGraphicsPixmapItem(parent) {}

    explicit HouseItem(const QPixmap& pixmap, QGraphicsItem* parent = nullptr) :
        QGraphicsPixmapItem(pixmap, parent) {}

    ~HouseItem();
};

